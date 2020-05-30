#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofApp.h"
#include "ofxGui.h"

const int RESOLUTION = 50;

SKETCH_BEGIN(k1_6, ofColor::blueViolet)
float field[RESOLUTION][RESOLUTION];
std::vector<glm::vec2> particles;
ofParameter<int> totalParticles = 500;
ofParameter<float> fieldScale = 0.1;
ofParameter<float> buffer = 100.;
ofParameter<int> numSteps = 50;
ofParameter<float> stepLength = 5;
ofParameterGroup controls;

ofColor backgroundColor() const {
	return ofColor::darkBlue;
}

	void setup() {


		controls.setName(name());
		controls.add(buffer.set("Margins", 100, 0, 500));
		parent->gui.add(controls);
	}

	void reset() {
		particles.clear();
		for (int i = 0; i < totalParticles; i++) {
			particles.push_back({ 0., 0. });
		}

		initializeFlowField();
		initializeParticlePositions();

		ofBackground(backgroundColor());
		ofSetBackgroundAuto(true);
		ofSetLineWidth(1);
	}

	void update() {
		//for (auto& p : particles) {
		//	int fx = p.x / RESOLUTION;
		//	int fy = p.y / RESOLUTION;
		//	float f = field[fx][fy];

		//	p.x += glm::cos(f);
		//	p.y += glm::sin(f);

		//	p.x = ofClamp(p.x, -ofGetWidth() / 2. + buffer, ofGetWidth() / 2. - buffer);
		//	p.y = ofClamp(p.y, -ofGetHeight() / 2. + buffer, ofGetHeight() / 2. - buffer);
		//}
	}

	void draw() {
		ofPushMatrix();
		ofNoFill();
		if (parent->showGui) {
			ofSetColor(255., 64.);
			drawField();
		}

		ofSetColor(labelColor());

		for (auto& p : particles) {
			drawCurve(p);
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
		float starting = ofRandomf();
		for (int x = 0; x < RESOLUTION; x++) {
			for (int y = 0; y < RESOLUTION; y++) {
				field[x][y] = ofMap(ofNoise(x * fieldScale, y * fieldScale), 0., 1., 0., 2.*PI);
			}
		}
	}

	void initializeParticlePositions() {
		float x_step = (ofGetWidth() - 2. * buffer) / (float)particles.size();
		float x = buffer;
		for (auto& p : particles) {
			p.x = x;
			p.y = ofRandom(ofGetHeight() - buffer * 2.) + buffer;

			x += x_step;
		}
	}

SKETCH_END