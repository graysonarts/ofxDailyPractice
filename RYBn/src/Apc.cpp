#include "Apc.h"

#define CONTROL_INDEX(channel, control) (channel << 8 | control)

Apc::Apc() {

}

Apc::~Apc() {
	turnOffAllLeds();
}

static unsigned char INITIALIZER_MESSAGE[] = {
	MIDI_SYSEX,
	0x47, 0x7F, 0x29, 0x60, 0x00, 0x04,
	0x41, // Ableton Live
	0x01, 0x01, 0x01, // Version
	MIDI_SYSEX_END
};

const unsigned char RED = 120;
const unsigned char GREEN = 122;
const unsigned char SOLID = 1;

void sendMidi(ofxMidiOut& out, unsigned char* bytes, int length) {
	out << StartMidi();
	for (int i = 0; i < length; i++) {
		out << bytes[i];
	}
	out << FinishMidi();
}

void turnOffLed(ofxMidiOut& out, unsigned channel, unsigned char note) {
	out.sendNoteOff(channel & 0x0F, note);
}

void Apc::setup() {
	ofLog() << "Midi In Ports";
	midiIn.listInPorts();
	midiIn.openPort(0);
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(true);

	ofLog() << "Midi Out Ports";
	midiOut.listOutPorts();
	midiOut.openPort(1);

	// Set to Ableton Live Mode
	ofLog(OF_LOG_VERBOSE) << "Setting Mode to Ableton Live";
	sendMidi(midiOut, INITIALIZER_MESSAGE, 12);

	ofLog(OF_LOG_VERBOSE) << "Clearing all leds";
	turnOffAllLeds();
	ofLog(OF_LOG_VERBOSE) << "Setting Ring Leds to Volume Style";
	setAllRingsToVolumeStyle();
}

void Apc::newMidiMessage(ofxMidiMessage& msg) {
	ButtonEntry* button;
	switch (msg.status) {
		case MIDI_NOTE_OFF:
			button = getButton(msg.pitch);
			if (button != nullptr && !button->isToggle) {
				midiOut.sendNoteOn(SOLID, msg.pitch, RED);
				button->state = false;
				button->callback(false);
			}
			break;
		case MIDI_NOTE_ON:
			button = getButton(msg.pitch);
			if (button != nullptr) {
				if (button->isToggle) {
					button->state = !button->state;
				}
				else {
					button->state = true;
				}
				midiOut.sendNoteOn(SOLID, msg.pitch, button->state ? GREEN : RED);
				button->callback(button->state);
			}
			break;
		case MIDI_CONTROL_CHANGE:
			ControlEntry* control = getControl(msg.channel, msg.control);
			if (control != nullptr) {
				control->value = msg.value;
				control->callback(control->value);
			}
			break;
	}
}

void Apc::registerControl(Control ctrl, std::function<void(bool)> callback) {
	switch (ctrl.type) {
		case ControlType::Momentary:
		case ControlType::Toggle:
			midiOut.sendNoteOn(SOLID, ctrl.note, RED);
			buttons[ctrl.note] = {
				ctrl.type == ControlType::Toggle,
				callback,
				false
			};
			break;
		default:
			ofLog(OF_LOG_ERROR) << "Control: " << ctrl.note << " is not a button control";
			break;
	}
}

void Apc::registerControl(Control ctrl, float defaultValue, std::function<void(float)> callback) {
	switch (ctrl.type) {
		case ControlType::Knob:
			midiOut.sendControlChange(ctrl.channel, ctrl.note, defaultValue);
		case ControlType::Slider:
			controls[CONTROL_INDEX(ctrl.channel, ctrl.note)] = { defaultValue, callback };
			break;
		default:
			ofLog(OF_LOG_ERROR) << "Control: " << ctrl.note << " is not a range control";
			break;
	}
}

ButtonEntry* Apc::getButton(unsigned char note) {
	auto& item = buttons.find(note);
	if (item == buttons.end()) {
		return nullptr;
	}

	return &(item->second);
}

ControlEntry* Apc::getControl(unsigned char channel, unsigned char control) {
	int c = CONTROL_INDEX(channel, control);
	auto& item = controls.find(c);
	if (item == controls.end()) {
		return nullptr;
	}

	return &(item->second);
}

void Apc::turnOffAllLeds() {
	// Clip Launch Buttons
	for (unsigned char n = 0x00; n <= 0x27; n++) {
		midiOut.sendNoteOff(SOLID, n, 0);
	}

	// Record Arm, Solo, Activator, Track Select, Track Stop, Clip Stop (E_3?)
	// And: Device Left, Device Right, Bank Left, Bank Right, Device ON/OFF, Device Lock, Clip/Device View, Detail view, Crossfader A/B
	// And: Master, Stop All Clips
	for (unsigned char n = 0x30; n <= 0x51; n++) {
		if (n <= 0x34 || n == 0x42) {
			for (unsigned char t = 0; t <= 7; t++) {
				midiOut.sendNoteOff(t, n, 0);
			}
		}
		else {
			midiOut.sendNoteOff(SOLID, n, 0);
		}
	}

	// Scene Launch, and rest
	for (unsigned char n = 0x52; n <= 0x67; n++) {
		midiOut.sendNoteOff(SOLID, n, 0);
	}
}

void Apc::setAllRingsToVolumeStyle() {
	// Device Knobs
	for (unsigned char n = 0x18; n <= 0x1F; n++) {
		midiOut.sendControlChange(1, n, 2);
	}

	for (unsigned char n = 0x38; n <= 0x3F; n++) {
		midiOut.sendControlChange(1, n, 2);
	}
}