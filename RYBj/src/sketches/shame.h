#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"

const string SHAME = "I did not do one today";

class shame : public IDailySketch {
private:
	const string m_name;
	const ofColor m_background;
	const ofColor m_label;
public:
	const string name() const {
		return m_name;
	}

	const ofColor labelColor() const {
		return m_label;
	}

	shame(ofApp* parent, string name, ofColor background, ofColor label) : IDailySketch(parent), m_name(name), m_background(background), m_label(label) { }

	void draw() {
		ofBackground(m_background);
		ofSetColor(labelColor());
		ofRectangle fontBox = parent->light.getStringBoundingBox(SHAME, 0, 0);
		fontBox.x = ofGetWidth() / 2. - fontBox.width / 2.;
		fontBox.y = ofGetHeight() / 2. - fontBox.height / 2.;

		parent->light.drawString(SHAME, fontBox.x, fontBox.y);
	}

	bool shortDuration() {
		return true;
	}
};