#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"

const string SHAME = "I did not do one today";

class j1_3 : public IDailySketch {
public:
	const string name() const {
		return "j1.3";
	}

	const ofColor labelColor() const {
		return ofColor::darkBlue;
	}

	j1_3(ofApp* parent) : IDailySketch(parent) { }

	void draw() {
		ofBackground(ofColor::aliceBlue);
		ofSetColor(labelColor());
		ofRectangle fontBox = parent->light.getStringBoundingBox(SHAME, 0, 0);
		fontBox.x = ofGetWidth() / 2. - fontBox.width / 2.;
		fontBox.y = ofGetHeight() / 2. - fontBox.height / 2.;

		parent->light.drawString(SHAME, fontBox.x, fontBox.y);
	}
};