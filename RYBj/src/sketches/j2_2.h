#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j2_2, ofColor::darkSlateGray)
std::vector<std::vector<glm::vec4> > curves;
size_t curveCount = 5;
size_t curvePoints = 16;
ofParameterGroup controls;
ofParameter<float> radius;

void setup() {
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	ofSetCircleResolution(100);

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
	ofSetLineWidth(3.);

	ofDrawCircle(0, 0, radius);
	//ofPushMatrix();
	//for (int i = 0; i < 360; i += 36) {
	//	ofSetColor(ofColor::red);
	//	parent->semibold.drawString(std::to_string(i), radius, 0);
	//	ofRotateDeg(36);
	//}
	//ofPopMatrix();

	for (const auto& curve : curves) {
		float x = 0, y = 0, angle = 0, r = 0; int index = 0;
		float lastx = 0, lasty = 0;
		ofBeginShape();
		for (const auto& pt : curve) {

			x = pt.x;
			y = pt.y + 100.*sin(ofGetElapsedTimeMillis() / 5000. * pt.z);



			ofSetColor(ofColor::darkGray, 64.);
			ofCurveVertex(x, y);

			if (parent->showGui) {
				if (lastx != 0 && lasty != 0) {
					ofDrawLine(lastx, lasty, x, y);
					ofDrawCircle(x, y, 5.);
				}

				lastx = x;
				lasty = y;

				ofSetColor(labelColor());
				parent->semibold.drawString(std::to_string(index++), x, y);
			}

		}
		ofSetColor(ofColor::darkGray, 64.);
		ofEndShape(true);
	}
	ofPopMatrix();
}

void update() {
	for (auto& curve : curves) {
		for (auto& pt : curve) {
			float angle = pt.w;
			if (angle > PI && angle < 2 * PI) {
				float noise = ofNoise(pt.x, pt.y, ofGetElapsedTimeMillis() / 5000.);
				pt.z = noise;
			}
		}
	}
}

private:
	std::vector<glm::vec4> randomized_curve(int index) {
		std::vector<glm::vec4> retval;
		ofLog() << "randomized curve";
		

		for (int i = 0; i < curvePoints; i++) {
			float degs = 360. / (float)curvePoints;
			float angle = glm::radians(degs) * (float)i;

			float x = radius * glm::cos(angle);
			float y = radius * glm::sin(angle);

			if (angle > PI && angle < 2 * PI) {
				//y = ofRandom(radius / 4. , -3.);
				y += 2. * ofRandom(radius, -3.) * -glm::sin(angle);
			}

			retval.push_back({ x, y, 0, angle });
		}

		return retval;
	}
	SKETCH_END