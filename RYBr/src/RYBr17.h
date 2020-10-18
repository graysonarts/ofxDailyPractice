#pragma once

#include "ofMain.h"
class ofApp;

const float SEGMENTS = 16;
const int BRANCH_LENGTH = 3;
const float MAX_ANGLE = 30;
const float MIN_LENGTH = 15.;
const float MAX_LENGTH = 75.;
const float DOT_SIZE = 4.;
const float LINE_WIDTH = 5.;

struct Design {
	float length;
	float degrees;
	std::unique_ptr<Design> left;
	std::unique_ptr<Design> right;

	Design(float l, float d) : length(l), degrees(d), left(nullptr), right(nullptr) { }
	inline const bool has_child() const { return left != nullptr || right != nullptr; }
};

class RYBr17
{
public:
	RYBr17(ofApp* app) : m_app(app), m_initialized(false), m_num_segments(SEGMENTS), m_segment_rotation(360. / (float)m_num_segments), m_segment(nullptr) { }

	void setup() {
		m_width = ofGetWidth();
		m_height = ofGetHeight();

		ofBackground(ofColor::antiqueWhite);

		reset();

		m_initialized = true;
	}

	void reset() {
		m_start_radius = ofRandom(m_height / 20.) + 10.;
		m_segment = std::make_unique<Design>(ofRandom(MAX_LENGTH - MIN_LENGTH) + MIN_LENGTH, ofRandom(-MAX_ANGLE, MAX_ANGLE));
		grow_branch(m_segment.get(), 1);
	}

	void grow_branch(Design* root, int count) {
		if (count > BRANCH_LENGTH) {
			return;
		}

		root->left = make_child();
		if (root->left != nullptr) {
			grow_branch(root->left.get(), count+1);
		}

		root->right = make_child();
		if (root->right != nullptr) {
			grow_branch(root->right.get(), count+1);
		}

	}

	std::unique_ptr<Design> make_child() {
		if (ofRandom(1.) < 0.75) {
			return std::make_unique<Design>(ofRandom(MAX_LENGTH - MIN_LENGTH) + MIN_LENGTH, ofRandom(-MAX_ANGLE, MAX_ANGLE));
		}

		return nullptr;
	}

	void update() {
		if (!isInitialized()) return;

	}
	void draw() {
		if (!isInitialized()) return;

		ofPushStyle();
		ofPushMatrix();

		ofTranslate(m_width / 2.0, m_height / 2.0);
		ofRotateDeg(180);
		ofSetColor(ofColor::dimGray);
		ofNoFill();
		ofSetLineWidth(LINE_WIDTH);
		ofDrawCircle(0, 0, m_start_radius);
		ofFill();

		if (m_segment != nullptr) {
			for (int i = 0; i < m_num_segments; i++) {
				ofRotateDeg(i * m_segment_rotation);
				draw_segment();
			}
		}

		ofPopMatrix();
		ofPopStyle();
	}

	void draw_segment() {
		ofPushMatrix();
		ofTranslate(0, m_start_radius);
		draw_step(m_segment.get(), 0);
		ofPopMatrix();
	}

	void draw_step(Design* root, int count) {
		if (root == nullptr) {
			return;
		}

		ofSetLineWidth(ofLerp(LINE_WIDTH, 1., (float)count / BRANCH_LENGTH));

		ofPushMatrix();
		ofDrawLine(0., 0., 0., root->length);
		ofTranslate(0, root->length);
		ofRotateDeg(root->degrees);

		ofDrawCircle(0., 0., DOT_SIZE);
		draw_step(root->left.get(), count + 1);
		draw_step(root->right.get(), count + 1);

		ofPopMatrix();
	}

	void set_padding_from_window_size(int w, int h) {
	}

	inline const bool isInitialized() const { return m_initialized; }

private:
	ofApp* m_app;
	bool m_initialized;
	int m_width, m_height;
	float m_start_radius;
	int m_num_segments;
	float m_segment_rotation;
	std::unique_ptr<Design> m_segment;
};

