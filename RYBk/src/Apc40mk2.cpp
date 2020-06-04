#include "Apc40mk2.h"

Apc40mk2::Apc40mk2(): initialized(false) {
}

Apc40mk2::~Apc40mk2() {
	if (initialized) {
		clear();

		mIn.closePort();
		mIn.removeListener(this);
	}
}

void Apc40mk2::setup() {
	mIn.openPort("APC40 mkII 0");
	mIn.addListener(this);
	mOut.openPort("APC40 mkII 0");

	initialized = true;
	//mIn.listInPorts();
	//mOut.listOutPorts();
}

void Apc40mk2::newMidiMessage(ofxMidiMessage& message) {

	ApcKey key = {message.control, message.channel, message.pitch };


	auto control = controls.find(key);
	if (control != controls.end()) {
		control->second->receive(message.velocity, message.value);
	}
	else {
		ofLog() << message.control << " c:" << message.channel << " p:" << message.pitch << " v:" << message.velocity << " :" << message.value;
	}
}


ApcCtrl* Apc40mk2::add(int type, int channel, int note, IReceiver& receiver, bool sendClear) {
	ApcCtrl* control = new ApcCtrl(type, channel, note, receiver);
	ApcKey key = { type, channel, note };
	controls[key] = control;

	if (sendClear) {
		mOut.sendControlChange(channel, type, 64.);
	}

	return control;
}

void Apc40mk2::clear() {
	for (auto c : controls) {
		ApcCtrl* ctl = c.second;
		delete ctl;
	}

	controls.clear();
}