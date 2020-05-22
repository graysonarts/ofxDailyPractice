#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

SKETCH_BEGIN(j2_5, ofColor::oldLace)
const int POINT_COUNT = 25;
std::vector<glm::vec2> points;
int point_index;
float last_point_time;
ofParameter<float> new_point_delay;
ofParameterGroup controls;

void setup() {
	ofEnableAlphaBlending();
	points.reserve(POINT_COUNT);
	controls.add(new_point_delay.set("new Point Interval", 1000, 500, 5000));
	parent->gui.add(controls);
}

void reset() {
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	float width = ofGetWidth() / 2.;
	float height = ofGetHeight() / 2.;

	points.clear();
	point_index = 0;
	last_point_time = 0;
	ofSetLineWidth(3.);
	for (int i = 0; i < POINT_COUNT; i++) {
		points.push_back({
			ofMap(ofRandom(-1, 1), -1, 1, -width, width, true),
			ofMap(ofRandom(-1, 1), -1, 1, -height, height, true)
		});
	}
}

void update() {
	float width = ofGetWidth() / 2.;
	float height = ofGetHeight() / 2.;

	float elapsed_time = ofGetElapsedTimeMillis() - last_point_time;

	if (elapsed_time > new_point_delay) {
		last_point_time = ofGetElapsedTimeMillis();
		glm::vec2 point = {
			ofMap(ofRandom(-1, 1), -1, 1, -width, width, true),
			ofMap(ofRandom(-1, 1), -1, 1, -height, height, true)
		};

		points[point_index % points.size()] = point;

		point_index = ++point_index % points.size();
	}


}

float ease(float percentage) {
	// easeInOutCirc from easing.net
	return (percentage < 0.5)
		? (1. - glm::sqrt(1. - glm::pow(2. * percentage, 2.))) / 2.
		: (glm::sqrt(1. - glm::pow(-2. * percentage + 2., 2.)) + 1.) / 2.;
}

glm::vec2 partial(glm::vec2 start_pt, glm::vec2 end_pt, float percentage) {
	glm::vec2 delta = end_pt - start_pt;
	float slope = delta.y / delta.x;

	delta.x = delta.x * ease(percentage);
	delta.y = delta.x * slope;


	return start_pt + delta;
}

void draw() {
	float percentage = (ofGetElapsedTimeMillis() - last_point_time) / new_point_delay;

	ofSetColor(ofColor::rosyBrown.r, ofColor::rosyBrown.g, ofColor::rosyBrown.b, 32.);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);



	int first_pt;
	int second_pt;

	for (int i = 0; i < points.size(); i++) {
		ofSetColor(labelColor());
		first_pt = (i + point_index - 1) % points.size();
		second_pt = (first_pt + 1) % points.size();
		if (first_pt == point_index || second_pt == point_index) {

		} 
		else if (first_pt == (point_index - 1) % points.size() || second_pt == (point_index - 1) % points.size()) {

		}
		else {

			ofDrawLine(points[first_pt], points[second_pt]);
		}
	}

	first_pt = (point_index - 1) % points.size();
	second_pt = (first_pt + 1 ) % points.size();


	glm::vec2 partial = this->partial(points[first_pt], points[second_pt], percentage);
	//ofSetColor(ofColor::purple);
	ofDrawLine(points[first_pt], partial);
	ofDrawCircle(points[second_pt], 3.);



	first_pt = (point_index) % points.size();
	second_pt = (first_pt + 1) % points.size();

	//ofSetColor(ofColor::red);

	partial = this->partial(points[second_pt], points[first_pt], 1. - percentage);
	ofDrawLine(points[second_pt], partial);

	//ofSetColor(ofColor::black);
	//ofDrawCircle(points[point_index], 5.);

	ofPopMatrix();
}

private:

	SKETCH_END