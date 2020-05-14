#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"

class j1_2 : public IDailySketch {
public:
	std::vector<std::vector<glm::vec2> > curves;
	size_t curveCount = 1;
	size_t curvePoints = 24;

	j1_2(ofApp* parent) : IDailySketch(parent) { }

	const string name() const {
		return "j1.2";
	}

	const ofColor labelColor() const {
		return ofColor::peru;
	}

	void setup() {
		ofEnableAlphaBlending();
		//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	}

	void reset() {
		curves.clear();
		for (int i = 0; i < curveCount; i++) {
			curves.push_back(randomized_curve(i+1));
		}
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
			ofBackground(ofColor::lightGray);
			ofSetColor(ofColor::darkGray, 128.);
			float x = 0, y = 0;
			for (const auto &curve : curves) {
				ofBeginShape();
				for (const auto& pt : curve) {
					x += pt.x * glm::cos(pt.y);
					y += pt.x * glm::sin(pt.y);
					ofCurveVertex(x, y);
				}
				ofEndShape(true);
				x = 0; y = 0;
				for (const auto &pt : curve) {
					ofSetColor(ofColor::red);
					ofDrawLine(0, 0, pt.x * glm::cos(pt.y), pt.x * glm::sin(pt.y));
					//x += pt.x * glm::cos(pt.y);
					//y += pt.x * glm::sin(pt.y);
					//ofDrawCircle(x, y, 3.);
				}
			}
		ofPopMatrix();
	}

	void update() {
		for (auto& curve : curves) {
			for (auto& pt : curve) {
				pt.x += (ofNoise(sin(ofGetElapsedTimeMillis() / 1000.)) - 0.5) * 10.;
				pt.y += (ofNoise(cos(ofGetElapsedTimeMillis() / 1423.)) - 0.5) * PI / 2.;
			}
		}
	}

private:
	std::vector<glm::vec2> randomized_curve(int index) {
		std::vector<glm::vec2> retval;
		float radiusMax = max(ofGetWidth() / 4. * (float)index / curvePoints, ofGetHeight() / 4. * (float)index / curvePoints);

		glm::vec2 start = {radiusMax * ofRandomf(), ofRandom(2 * PI) };

		retval.push_back(start);

		for (int i = 0; i < curvePoints - 2; i++) {
			float currentRadius = ofRandom(10.0f);
			float angle = ofRandom(glm::radians(-90.), glm::radians(90.));
			retval.push_back({ currentRadius, angle });
		}

		retval.push_back(start);

		return retval;
	}
};