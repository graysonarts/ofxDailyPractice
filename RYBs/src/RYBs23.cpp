#include "ofApp.h"

void appRYBs23::setup() {
	seed = ofRandomf() * 65000;
	ofSeedRandom(seed);

	gui.setup("Tuning");
	main_size = 250;
	//gui.add(main_size.set("Size", 250, 10, ofGetWidth()));
	gui.add(offset.set("Offset", 70, 10, ofGetWidth()));
	gui.add(strip_width.set("Strip Width", 25, 10, 50));

	height = sqrt(main_size * main_size - (main_size / 2.0) * (main_size / 2.0));

	mask.allocate(ofGetWidth(), ofGetHeight(), GL_LUMINANCE);
	drawing.allocate(ofGetWidth(), ofGetHeight());
}

void appRYBs23::update() {
	triangle.clear();
	triangle.moveTo(0, height);
	triangle.lineTo(main_size, -height);
	triangle.lineTo(-main_size, -height);
	triangle.lineTo(0, height);

	mask.begin();
	ofTranslate(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
	ofScale(1, -1);
	ofClear(ofColor::black);
	triangle.draw();
	ofSetColor(ofColor::black);
	ofTranslate(0, -offset);
	draw_section();
	ofRotateDeg(120);
	draw_section();
	ofRotateDeg(120);
	draw_section();
	mask.end();

	drawing.begin();
	ofTranslate(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
	ofScale(1, -1);
	ofClear(ofColor::orange);

	// TODO: Create Paper Texture
	drawing.end();

	drawing.getTexture().setAlphaMask(mask.getTexture());
}

void appRYBs23::draw_section() {
	float spacing = main_size / 5.0 - strip_width;
	for (int i = 0; i < 5; i++) {
		ofTranslate(-spacing * i, 0);
		ofDrawRectangle(0, -strip_width, -strip_width, main_size * 2);
	}
}

void appRYBs23::draw() {
	drawing.draw(0, 0);

	gui.draw();
	
}