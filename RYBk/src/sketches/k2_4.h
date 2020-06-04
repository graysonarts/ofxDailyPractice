#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(k2_4, ofColor::bisque)
float knobs[3];
float lens[3];
float angles[3];
int stepCount = 300;

ofColor background() const {
	return ofColor(0x123456);
}

bool triggered = false;
	void setup() {
	}

	void reset() {
		// Clear Screen Clip Launch 1
		parent->midi.add(0, 1, 0, *this);
		
		// 1-3 sliders - Length
		parent->midi.add(7, 1, 0,  *this);
		parent->midi.add(7, 2, 0, *this);
		parent->midi.add(7, 3, 0, *this);
		
		// 1-3 knobs - angle speed
		parent->midi.add(48, 1, 0, *this, true);
		parent->midi.add(49, 1, 0, *this, true);
		parent->midi.add(50, 1, 0, *this, true);

		// Master Count Step
		parent->midi.add(14, 1, 0, *this);

		ofSetBackgroundAuto(false);
		ofClear(background());
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);

		for (int i = 0; i < 3; i++) {
			knobs[i] = 64.; lens[i] = 10.; angles[i] = 0;
		}

		ofSetLineWidth(2.0);
	}

	void update() {
		if (triggered) {
			updateStep();
		}
	}

	void drawStep() {
		glm::vec2 location = { 0, 0 };

		for (int i = 0; i < 3; i++) {
			glm::vec2 lastLocation = location;
			location += { lens[i] * glm::sin(angles[i]), lens[i] * glm::cos(angles[i]) };

			if (triggered) {
				ofDrawLine(lastLocation, location);
			}
			ofDrawCircle(location, 1.);
		}

		ofDrawCircle(location, 5.);
	}

	void updateStep() {
		for (int i = 0; i < 3; i++) {
			angles[i] += ofMap(knobs[i], 0., 127.0, -.001 * (i + 10), .001 * (i + 10));
			angles[i] = ofWrapRadians(angles[i]);

		}
	}

	void draw() {
		//if (triggered) {
			//ofBackground(background());
		//	triggered = false;
		//}
		ofFill();
		ofSetColor(background(), 25.);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);

		ofSetColor(labelColor(), 64.);

		if (triggered) {
			drawStep();
		}
		else {
			for (int steps = 0; steps < stepCount; steps++) {
				drawStep();
				updateStep();
			}
		}

		//ofSetColor(ofColor::blueViolet);
		//ofDrawRectangle(0, 0, 400., 100.);
		//ofSetColor(labelColor());
		//string label = "1: " + std::to_string(knobs[0]) + ", " + std::to_string(lens[0]) + ", " + std::to_string(angles[0]);
		//parent->semibold.drawString(label, 0, 0);
		ofPopMatrix();
	}

	virtual void trigger(int control, int channel, int note, bool state) {
		triggered = state;
	}

	virtual void setValue(int control, int channel, int note, int value) {
		switch (control) {
			case 7:
				lens[channel - 1] = ofMap(value, 0., 127., 10., 300.); break;
			case 14:
				stepCount = ofMap(value, 0, 127, 100, 10000); break;
			case 48:
			case 49:
			case 50:
				knobs[control-48] = value; break;
			default:
				ofLog() << "ctrl(" << control << ") ch(" << channel << ") n(" << note << ") v(" << value << ")";
				break;
		}
	}

private:

SKETCH_END