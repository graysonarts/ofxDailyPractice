#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j1_6, ofColor::darkOrchid)

ofParameter<int> circleCount = 36;
ofParameter<float> buffer = 25;
ofParameter<float> minAxisLength = 10;
std::vector<float> circlePos; // x is x position, y is minor axis length (because it's an ellipse
ofShader chroma;
ofSoundPlayer sound;
float lastPlayTime = 0;

void setup() {
	sound.load("wavs/j1_6.wav");
	sound.setMultiPlay(true);
}

void reset() {
	circlePos.clear();
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_SCREEN);

	for (int i = 0; i < circleCount; i++) {
		float x = glm::sin((float)i/circleCount * 2. * PI);
		circlePos.push_back(x);
	}
}

void stop() {
	ofLog() << "Stopping sound";
	sound.stop();
}


void draw() {
	float time = ofGetElapsedTimef();
	if (time - lastPlayTime > 2) {
		sound.play();
	}
	ofEnableSmoothing();
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);

	ofBackground(ofColor::ghostWhite);
	ofBackground(ofColor::darkGray);

	for (const auto& circle : circlePos) {
		ofColor color = ofColor::whiteSmoke;
		color.setBrightness(255. * glm::abs(circle));
		ofSetColor(color, 64.);
		float x = circle + glm::sin(ofGetElapsedTimeMillis() / 1000.);
		float offset = 1 - glm::abs(circle + glm::sin(ofGetElapsedTimeMillis() / 500.));
		float width = glm::max((float)(ofGetHeight() / 4. * offset), minAxisLength.get());

		// Scale x appropriately
		x *= (ofGetWidth()-buffer - width) / 2.;
		//x = circle * ofGetWidth() / 2.;

		ofDrawEllipse(x, 0, width, ofGetHeight() / 4.);
	}

	ofPopMatrix();
}

SKETCH_END