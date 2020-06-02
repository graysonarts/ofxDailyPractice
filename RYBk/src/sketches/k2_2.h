#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(k2_2, ofColor::bisque)
	void setup() {
	}

	void reset() {
	}

	void update() {
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
		ofBackground(ofColor::blueViolet);
		ofPopMatrix();
	}

private:

SKETCH_END