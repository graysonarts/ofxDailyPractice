#pragma once

#include <unorderd_map>

#include "ofMain.h"
#include "ofxMidi.h"

typedef void (*Setter)(int);

class IReceiver {
public:
	virtual ~IReceiver() { }
};

class ApcCtrl {
public:
	const int channel, note, outChannel, outNote;
	const Setter& setter;
	ofxMidiOut& out;


	ApcCtrl(int channel, int note, Setter& setter, int outChannel, int outNote, ofxMidiOut& out)
		: channel(channel), note(note), setter(setter), out(out), outChannel(outChannel), outNote(outNote) {

	}

	void receive(int velocity) {
		setter(velocity);
	}

	void send(int velocity = 64) {
		out.sendNoteOn(outChannel, outNote, velocity);
	}

	void clear() {
		out.sendNoteOff(outChannel, outNote);
	}
};

struct ApcKey {
	int channel;
	int note;
};

class Apc40mk2 : public ofxMidiListener
{
	ofxMidiIn mIn;
	ofxMidiOut mOut;
	bool initialized;
	std::unordered_map<ApcKey, ApcCtrl*> controls;

public:
	Apc40mk2();
	~Apc40mk2();

	void setup();
	virtual void newMidiMessage(ofxMidiMessage& message);

	void add(int channel, int note, int outChannel, int outNote, Setter& setter);
	void clear();
};

