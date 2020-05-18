#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j1_7, ofColor::darkMagenta)
ofParameter<float> maxRadius;
ofParameterGroup controls;

	void setup() {
		controls.setName(name());
		controls.add(maxRadius.set("maxRadius", 300, 20, ofGetHeight() / 2.));

		parent->gui.add(controls);
	}

	void reset() {
		points.clear();
	}

	bool isInteractive() { return true;  }

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
		ofBackground(ofColor::plum);
		ofSetColor(ofColor::floralWhite, 128.);
		ofDrawCircle(currentPoint, 10.);
		ofDrawCircle(0, 0, maxRadius);

		ofSetColor(labelColor(), 128.);
		ofDrawLine(-100, 0, 100, 0);
		ofDrawLine(0, -100, 0, 100);
		//for (float x = -10; x < 10; x++) {
		//	for (float y = -10; y < 10; y++) {
		//		if (x < 0 || y < 0) {
		//			ofSetColor(ofColor::floralWhite, 128.);
		//		}
		//		else {
		//			ofSetColor(labelColor(), 128.);
		//		}
		//		ofDrawBitmapString(std::to_string(glm::atan(y , x)), x*100., y*100.);
		//		ofDrawCircle(constrain(x*100., y*100.), 2.);
		//	}
		//}

		glm::vec2 bps[4];
		int head = 0, count = 0;
		for (const auto& pt : points) {
			bps[head++] = pt;
			head %= 4;
			if (count++ > 3) {
				ofDrawBezier(
					bps[head].x, bps[head].y,
					bps[(head + 1) % 4].x, bps[(head + 1) % 4].y,
					bps[(head + 2) % 4].x, bps[(head + 2) % 4].y,
					bps[(head + 3) % 4].x, bps[(head + 3) % 4].y
				);
			}
			else {
				for (int i = 0; i < head; i++) {
					ofDrawCircle(bps[i], 2.);
				}
			}
			//float r = glm::sqrt(pt.x * pt.x + pt.y * pt.y);
			//ofDrawBitmapString(std::to_string(r), pt);
		}

		ofPopMatrix();
	}

	void onButton(float x, float y) {
		x -= ofGetWidth() / 2.;
		y -= ofGetHeight() / 2.;
		points.push_back(constrain(x, y ));
	}

	void onMove(float x, float y) {
		x -= ofGetWidth() / 2.;
		y -= ofGetHeight() / 2.;
		currentPoint = constrain(x, y);
	}

	glm::vec2 constrain(float x, float y) {
		float r = glm::sqrt(x * x + y * y);
		//float t = glm::sign(x) * glm::atan(glm::abs(y / x));
		float t = glm::atan(y, x);
		

		r = min(r, maxRadius.get());

		return { r * glm::cos(t), r * glm::sin(t) };
	}


private:
	std::vector<glm::vec2> points;
	glm::vec2 currentPoint;

SKETCH_END