#pragma once

#include <unordered_map>

#include "ofMain.h"
#include "ofxMidi.h"
#include "ApcControls.h"


enum class ControlType {
	Toggle,
	Momentary,
	Slider,
	Knob
};

struct Control {
	ControlType type;
	unsigned char note;
	unsigned char channel;
};

struct ButtonEntry {
	bool isToggle;
	std::function<void(bool)> callback;
	bool state;
};

struct ControlEntry {
	float value;
	std::function<void(float)> callback;
};

class Apc : public ofxMidiListener
{
public:
	Apc();
	virtual ~Apc();
	void setup();

	void newMidiMessage(ofxMidiMessage& eventArgs);
	void registerControl(Control ctrl, float defaultValue, std::function<void(float)> callback);
	void registerControl(Control ctrl, std::function<void(bool)> callback);

private:
	ofxMidiIn midiIn;
	ofxMidiOut midiOut;

	unordered_map<int, ButtonEntry> buttons;
	unordered_map<int, ControlEntry> controls;

	void turnOffAllLeds();
	void setAllRingsToVolumeStyle();

	ButtonEntry* getButton(unsigned char note);
	ControlEntry* getControl(unsigned char channel, unsigned char control);
};

