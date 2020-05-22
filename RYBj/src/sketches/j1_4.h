#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j1_4, ofColor::cornflowerBlue)
	std::vector<std::vector<glm::vec2> > curves;
	size_t curveCount = 5;
	size_t curvePoints = 16;
	ofParameterGroup controls;
	ofParameter<float> radius;

	void setup() {
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

		controls.setName(name());
		controls.add(radius.set("Radius", 250, 100, ofGetHeight()));

		parent->gui.add(controls);
	}

	void reset() {
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

		curves.clear();
		for (int i = 0; i < curveCount; i++) {
			curves.push_back(randomized_curve(i + 1));
		}
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);

		ofClear(ofColor::lightGray);
		ofSetColor(ofColor::darkGray, 64.);

		for (const auto& curve : curves) {
			float x = 0, y = 0, angle = 0, r = 0; int index = 0;
			ofBeginShape();
			for (const auto& pt : curve) {
				angle += pt.y;
				r += pt.x;

				if (r > radius) {
					r = radius;
				}
				if (r < -radius) {
					r = -radius;
				}

				x = r * glm::cos(angle);
				y = r * glm::sin(angle);

				ofCurveVertex(x, y);
				//parent->semibold.drawString(std::to_string(index++), x, y);

			}
			ofEndShape(true);
		}
		ofPopMatrix();
	}

	void update() {
		for (auto& curve : curves) {
			for (auto& pt : curve) {
				float noise = ofNoise(ofGetElapsedTimeMillis() / 239875.) * 2 * PI;
				pt.x += sin(noise);
				pt.y += cos(noise)/5000.0;
				//pt.x -= (ofNoise(sin(ofGetElapsedTimeMillis() / 83405.)) - 0.5) * 10.;
				//pt.y += (ofNoise(cos(ofGetElapsedTimeMillis() / 1423.)) - 0.5) * PI / 2.;
			}
		}
	}

private:
	std::vector<glm::vec2> randomized_curve(int index) {
		std::vector<glm::vec2> retval;
		float radiusMax = max(ofGetWidth() / 4. * (float)index / curvePoints, ofGetHeight() / 4. * (float)index / curvePoints);

		glm::vec2 start = { radiusMax * ofRandomf(), ofRandom(2 * PI) };

		retval.push_back(start);

		for (int i = 0; i < curvePoints - 2; i++) {
			float currentRadius = ofRandom(500.*i / curvePoints + 100.);
			float angle = ofRandom(0., glm::radians(i * 10. ));
			retval.push_back({ currentRadius, angle });
		}

		retval.push_back(start);

		return retval;
	}
SKETCH_END