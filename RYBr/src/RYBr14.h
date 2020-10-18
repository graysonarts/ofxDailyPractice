#pragma once

#include "ofMain.h"
#include "Grid.h"

class ofApp;

class RYBr14
{
public:
	RYBr14(ofApp* app) : m_app(app), m_width(-1), m_height(-1), m_cell_size(-1), m_grid(nullptr), m_padding_x(0), m_padding_y(0) { }
	void setup(int cell_size) { 
		int screen_width = ofGetWidth();
		int screen_height = ofGetHeight();
		m_cell_size = cell_size;
		m_width = screen_width / m_cell_size;
		m_height = screen_height / m_cell_size;
		
		set_padding_from_window_size(screen_width, screen_height);

		// Previous grid should be deleted by the use of Unique Ptr
		m_grid = make_unique<Grid>(m_width, m_height);
	}
	void update() {
		if (!isInitialized()) return;

	}
	void draw() {
		if (!isInitialized()) return;

		ofPushStyle();
		ofPushMatrix();
		ofTranslate(m_padding_x, m_padding_y);
		//ofNoFill();

		m_grid->for_each([&](int x, int y, int d) {
			ofSetColor(d, d, d);
			x = x * m_cell_size;
			y = y * m_cell_size;

			ofDrawRectangle(x, y, m_cell_size, m_cell_size);
		});

		ofPopMatrix();
		ofPopStyle();
	}

	void set_padding_from_window_size(int w, int h) {
		m_padding_x = (w - m_width * m_cell_size) / 2;
		m_padding_y = (h - m_height * m_cell_size) / 2;
	}

	const bool isInitialized() const { return m_cell_size > 0;  }

private:
	ofApp *m_app;
	int m_width, m_height, m_cell_size;
	std::unique_ptr<Grid> m_grid;
	int m_padding_x, m_padding_y;

};

