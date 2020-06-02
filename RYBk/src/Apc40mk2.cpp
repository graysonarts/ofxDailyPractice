#include "Apc40mk2.h"

Apc40mk2::Apc40mk2() {
}

Apc40mk2::~Apc40mk2() {
	if (initialized) {
		clear();

		mIn.closePort();
		mIn.removeListener(this);
		mOut.closePort();
	}
}

void Apc40mk2::setup() {
	mIn.openPort("APC40 mkII 0");
	mIn.addListener(this);
	mOut.openPort("APC40 mkII 1");

	initialized = true;
	//mIn.listInPorts();
	//mOut.listOutPorts();
}

void Apc40mk2::newMidiMessage(ofxMidiMessage& message) {
	ofLog() << message.channel << " p:" << message.pitch << " v:" << message.velocity;
}

void Apc40mk2::add(int channel, int note, int outChannel, int outNote, Setter& setter) {
	ApcCtrl* control = new ApcCtrl(channel, note, setter, outChannel, outNote, mOut);
	control->send(5);
	ApcKey key = { channel, note };
	controls[key] = control;
}

void Apc40mk2::clear() {
	for (auto c : controls) {
		ApcCtrl* ctl = c.second;
		ctl->clear();
		delete ctl;
	}

	controls.clear();
}