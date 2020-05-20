#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j2_3, ofColor::blueSteel)
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

void draw_explore() {
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
	ofBackground(ofColor::lightGray);
	float angle = 0, x = 0, y = 0;

	for (int i = 0; i < 5; i++) {
		ofSetColor(ofColor::red, 128.);
		angle = min(2 * PI, PI + glm::abs(glm::log2(PI / 8. * (float)(i + 1))));
		 x = radius * glm::cos(angle);
		 y = radius * glm::sin(angle);
		ofDrawCircle(x, y, 10.);

		angle = min(2 * PI, 2 * PI - glm::abs(glm::log2(PI / 8. * (float)(i + 1))));
		 x = radius * glm::cos(angle);
		 y = radius * glm::sin(angle);
		ofDrawCircle(x, y, 10.);
	}

	ofPopMatrix();
}

void draw() {
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);

	ofBackground(ofColor::lightGray);
	ofSetColor(ofColor::dimGrey, 64.);
	ofSetLineWidth(3.);

	ofDrawCircle(0, 0, radius);

	ofRotateDeg(45. * sin(ofGetElapsedTimeMillis() / 100000.));

	for (const auto& curve : curves) {
		float x = 0, y = 0, angle = 0, r = 0; int index = 0;
		float lastx = 0, lasty = 0;
		ofBeginShape();
		for (const auto& pt : curve) {

			x = pt.x;
			y = pt.y + 100. * sin(ofGetElapsedTimeMillis() / 2000. * pt.z);



			ofSetColor(ofColor::steelBlue, 64.);
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
		ofSetColor(ofColor::steelBlue, 64.);
		ofEndShape(true);
	}
	ofPopMatrix();
}

void update() {
	float index = 0;
	for (auto& curve : curves) {
		int i = 0;
		float angle_start = min(2 * PI, PI + glm::abs(glm::log2(PI / 8. * (float)(index + 1))));
		float angle_end = min(2 * PI, 2 * PI - glm::abs(glm::log2(PI / 8. * (float)(index + 1))));
		for (auto& pt : curve) {
			float angle = pt.w;


			if (angle > angle_start && angle < angle_end) {
				float noise = ofNoise(pt.x, pt.y, ofGetElapsedTimeMillis() / 5000.);
				pt.z = noise;
			}
			i++;
		}
		index++;
	}
}

private:
	std::vector<glm::vec4> randomized_curve(int index) {
		std::vector<glm::vec4> retval;
		ofLog() << "randomized curve";
		float angle_start = min(2 * PI, PI + glm::abs(glm::log2(PI / 8. * (float)(index + 1))));
		float angle_end = min(2 * PI, 2 * PI - glm::abs(glm::log2(PI / 8. * (float)(index + 1))));
		float degs = 360. / (float)(curvePoints - 3);

		glm::vec2 noise = { ofRandom(radius), ofRandom(radius) };

		for (int i = 0; i < curvePoints; i++) {
			float angle = glm::radians(degs) * (float)i;

			float x = radius * glm::cos(angle);
			float y = radius * glm::sin(angle);



			if (angle > angle_start && angle < angle_end) {
				//y = ofRandom(radius / 4. , -3.);
				//y += 2. * ofRandom(radius, -3.) * -glm::sin(angle);
				float offset = ofNoise(noise) * 2 * radius * -glm::sin(angle);
				y += offset;
				noise += { (float)(x + (1 + index)*25.), (float)(y + (1 + index) * 25.) };
			}

			retval.push_back({ x, y, 0, angle });
		}

		return retval;
	}
	SKETCH_END