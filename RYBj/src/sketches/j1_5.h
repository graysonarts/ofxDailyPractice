#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j1_5, ofColor::cornflowerBlue)
	std::vector<glm::vec2> curves;
	size_t curvePoints = 360;
	ofParameterGroup controls;
	ofParameter<float> minRadius, maxRadius, buffer, lineWidth;

	void setup() {
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

		controls.setName(name());
		controls.add(minRadius.set("minRadius", 250, 10, ofGetHeight()/4.));
		controls.add(maxRadius.set("maxRadius", 500, 20, ofGetHeight() / 2.));
		controls.add(buffer.set("buffer", 25, 20, ofGetHeight() / 4.));
		controls.add(lineWidth.set("width", 1, 1, 23));

		parent->gui.add(controls);
	}

	void reset() {
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

		curves.clear();
		curves = randomize(0);
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
		ofRotateDeg(ofGetElapsedTimeMillis()/10000.);
		ofSetLineWidth(lineWidth);

		ofClear(ofColor::lightGray);
		ofSetColor(ofColor::darkGray, 64.);

		for (const auto& pt : curves) {

				ofSetColor(ofColor::darkGray, 64.);
				float angle = pt.y;
				float r = pt.x;

				if (r > maxRadius) {
					r = maxRadius;
				}
				if (r < minRadius) {
					r = minRadius;
				}

				r += r * ofNoise(r + ofGetElapsedTimeMillis() / 9000., angle + ofGetElapsedTimeMillis() / 5000.);

				float x = r * glm::cos(angle);
				float y = r * glm::sin(angle);
				float x_0 = (minRadius - buffer) * glm::cos(angle);
				float y_0 = (minRadius - buffer) * glm::sin(angle);
				ofDrawLine(x_0, y_0, x, y);
		}
		ofPopMatrix();
	}


private:
	std::vector<glm::vec2> randomize(int index) {
		std::vector<glm::vec2> retval;
		float radiusMax = max(ofGetWidth() / 4. * (float)index / curvePoints, ofGetHeight() / 4. * (float)index / curvePoints);

		glm::vec2 start = { radiusMax * ofRandomf(), ofRandom(2 * PI) };

		retval.push_back(start);

		float angle = 0;
		for (int i = 0; i < curvePoints - 2; i++) {
			float currentRadius = ofRandom(500.*i / curvePoints + 100.);
			angle += ofRandom(0., glm::radians(i * 10. ));
			retval.push_back({ currentRadius, angle });
		}

		retval.push_back(start);

		return retval;
	}
SKETCH_END