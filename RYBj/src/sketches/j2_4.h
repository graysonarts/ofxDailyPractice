#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

const int POINT_COUNT = 50;

SKETCH_BEGIN(j2_4, ofColor::white)

std::vector<glm::vec2> points;
	int point_index;
	float last_point_time;
	ofParameter<float> new_point_delay;
	ofParameterGroup controls;

	void setup() {
		points.reserve(POINT_COUNT);
		controls.add(new_point_delay.set("new Point Interval", 1000, 500, 5000));
	}

	void reset() {
		points.clear();
		point_index = 0;
		last_point_time = 0;
		ofSetLineWidth(3.);
	}

	void update() {
		float width = ofGetWidth() / 2.;
		float height = ofGetHeight() / 2.;

		float elapsed_time = ofGetElapsedTimeMillis() - last_point_time;

		if (elapsed_time > new_point_delay) {
			++point_index;
			glm::vec2 point = {
				ofMap(ofRandom(-1, 1), -1, 1, -width, width, true),
				ofMap(ofRandom(-1, 1), -1, 1, -height, height, true)
			};

			if (point_index >= points.size() && points.size() < POINT_COUNT) {
				points.push_back(point);
			}
			else {
				points[point_index % points.size()] = point;
			}
		}
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
		ofBackground(ofColor::black);

		ofSetColor(labelColor());
		for (int i = 1; i < points.size(); i++) {
			int first_pt = (i + point_index - 1) % points.size();
			int second_pt = (first_pt + 1) % points.size();
			ofDrawLine(points[first_pt], points[second_pt]);
		}

		ofPopMatrix();
	}

private:

SKETCH_END