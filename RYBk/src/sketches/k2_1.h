#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(k2_1, ofColor::orange)
ofParameter<float> outer = 50.f;
ofParameter<float> inner = outer * 0.866025404;
glm::vec2 hexFactor = { inner * 2., outer * 1.5f };
PaletteSource* colors;

void drawHex(glm::vec2 ct, ofColor fill) {
	glm::vec2 gridPos = ct;
	gridPos += { ct.y * 0.5f - (int)(ct.y / 2), 0.};
	gridPos *= hexFactor;

	ofPushMatrix();
	ofTranslate(gridPos);

	ofSetColor(fill);
	ofBeginShape();
	ofVertex(0., outer);
	ofVertex(inner, .5 * outer);
	ofVertex(inner, -.5 * outer);
	ofVertex(0., -outer);
	ofVertex(-inner, -.5 * outer);
	ofVertex(-inner, .5 * outer);
	ofEndShape(true);

	ofSetColor(ofColor::black);
	string label = "(" + std::to_string(ct.x).substr(0, 4) + "," + std::to_string(ct.y).substr(0, 4) + ")";
	auto box = parent->semibold.getStringBoundingBox(label, 0, 0);
	parent->semibold.drawString(label, -box.width / 2., 0);
	ofPopMatrix();
}

	void setup() {
		colors = new PaletteSource("colors/Celestial.jpg");
	}

	void reset() {
		ofFill();
		ofSetLineWidth(3.);
		
	}

	void update() {
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
		ofBackground(ofColor::lightCyan);

		float colorStep = (float)colors->numColors().x / 49.f;;

		for (int x = -3; x <= 3; x++) {
			for (int y = -3; y <= 3; y++) {
				ofFill();
				drawHex({ x, y }, colors->getColorAt((x + 3) * (y + 3) * colorStep, 0));
				ofNoFill();
				drawHex({ x, y }, ofColor::black);
			}
		}
		ofPopMatrix();
	}

private:

SKETCH_END