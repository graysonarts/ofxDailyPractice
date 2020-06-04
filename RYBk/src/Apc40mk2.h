#pragma once

#include <unordered_map>

#include "ofMain.h"
#include "ofxMidi.h"



class IReceiver {
public:
	virtual void trigger(int control, int channel, int note, bool state) { }
	virtual void setValue(int control, int channel, int note, int value) { }
	virtual ~IReceiver() { }
};

class ApcCtrl {
public:
	const int type, channel, note;
	IReceiver* receiver;


	ApcCtrl(int type, int channel, int note, IReceiver& receiver)
		: type(type), channel(channel), note(note), receiver(&receiver) {

	}

	void receive(int velocity, int value) {
		switch (type) {
		case 0:
			receiver->trigger(type, channel, note, velocity == 127); break;
		default:
			receiver->setValue(type, channel, note, value); break;
		}
	}
};

struct ApcKey {
	int type;
	int channel;
	int note;

	bool operator==(const ApcKey& p) const {
		return type == p.type && channel == p.channel && note == p.note;
	}
};

struct HashApcKey {
	std::size_t operator()(const ApcKey& key) const {
		return std::hash<int>()(key.channel) ^ std::hash<int>()(key.note) ^ std::hash<int>()(key.type);
	}
};

class Apc40mk2 : public ofxMidiListener
{
	ofxMidiIn mIn;
	ofxMidiOut mOut;
	bool initialized;
	std::unordered_map<ApcKey, ApcCtrl*, HashApcKey> controls;

public:
	Apc40mk2();
	~Apc40mk2();

	void setup();
	virtual void newMidiMessage(ofxMidiMessage& message);
	ApcCtrl* add(int type, int channel, int note, IReceiver& receiver, bool sendClear = false);

	void clear();
private:
};

