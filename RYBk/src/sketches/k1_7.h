#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"
#include "palette.h"

struct Particle {
	ofColor color;
	glm::vec2 position;
};

SKETCH_BEGIN(k1_7, ofColor::fireBrick)
static const int RESOLUTION = 50;
float field[RESOLUTION][RESOLUTION];
std::vector<Particle> particles;
ofParameter<int> totalParticles = 500;
ofParameter<float> fieldScale = 0.0015;
ofParameter<float> buffer = 100.;
ofParameter<int> numSteps = 100;
ofParameter<float> stepLength = 5;
ofParameter<float> changeFrequency = 1000.;
ofParameterGroup controls;
PaletteSource *colors;

ofColor backgroundColor() const {
	return ofColor::ivory;
}

	void setup() {
		colors = new PaletteSource("colors/Celestial.jpg");

		controls.setName(name());
		controls.add(buffer.set("Margins", 100, 0, 500));
		controls.add(fieldScale.set("Scale", 0.0015, 0., 0.1));
		controls.add(numSteps.set("Steps", 100, 10, 500));
		controls.add(stepLength.set("Step Len", 5., 1., 200.));
		controls.add(changeFrequency.set("Speed", 1000., 1., 10000.));
		parent->gui.add(controls);


	}

	void reset() {
		particles.clear();
		for (int i = 0; i < totalParticles; i++) {
			particles.push_back({ labelColor(), { 0., 0.} });
		}

		initializeFlowField();
		initializeParticlePositions();

		ofBackground(backgroundColor());
		ofSetBackgroundAuto(true);
		ofSetLineWidth(1);
	}

	void update() {
		float timeValue = ofGetElapsedTimeMillis()/changeFrequency;
		for (int x = 0; x < RESOLUTION; x++) {
			for (int y = 0; y < RESOLUTION; y++) {
				field[x][y] += 0.01;
			}
		}

		for (auto& p : particles) {
			float disturbance = ofMap(ofNoise(p.position.x * fieldScale, p.position.y*fieldScale, timeValue), 0., 1., 0, 10.);
			p.position.x += disturbance * glm::cos(timeValue);
			p.position.y += disturbance * glm::sin(timeValue);
		}
	}

	void draw() {
		ofPushMatrix();
		ofNoFill();
		if (parent->showGui) {
			ofSetColor(0., 64.);
			drawField();
		}

		for (auto& p : particles) {
			ofSetColor(p.color);
			drawCurve(p.position);
		}

		ofPopMatrix();
	}

	void drawField() {
		float step_x = ofGetWidth() / RESOLUTION;
		float step_y = ofGetHeight() / RESOLUTION;

		for (int x = 0; x < RESOLUTION; x++) {
			for (int y = 0; y < RESOLUTION; y++) {
				ofPushMatrix();
				ofTranslate(x * step_x + step_x/2., y * step_y + step_y/2.);
				ofRotateRad(field[x][y]);
				ofDrawLine(0, -5, 0, 5);
				ofPopMatrix();
			}
		}
	}

	void drawCurve(const glm::vec2& p) {

		float x = p.x, y = p.y;
		ofBeginShape();

		for (int i = 0; i < numSteps; i++) {
			ofCurveVertex(x, y);
			int fx = x / RESOLUTION;
			int fy = y / RESOLUTION;

			float angle = field[fx][fy];
			x += stepLength * glm::cos(angle);
			y += stepLength * glm::sin(angle);

			x = ofClamp(x, buffer, ofGetWidth() - buffer);
			y = ofClamp(y, buffer, ofGetHeight() - buffer);
		}
		ofEndShape();
	}

private:
	void initializeFlowField() {

		for (int x = 0; x < RESOLUTION; x++) {
			float starting = ofRandom(2. * PI);
			for (int y = 0; y < RESOLUTION; y++) {
				field[x][y] = starting;
				starting += ofMap(ofRandomf(), 0., 1., -PI/8., PI/8.);
			}
		}
	}

	void initializeParticlePositions() {
		float x_step = (ofGetWidth() - 2. * buffer) / (float)particles.size();
		float x = buffer;
		for (auto& p : particles) {
			p.position.x = x;
			p.position.y = ofRandom(ofGetHeight() - buffer * 2.) + buffer;
			if (colors != nullptr) {
				p.color = colors->getColorAt(p.position.x, 0.);
			}

			x += x_step;
		}
	}

SKETCH_END