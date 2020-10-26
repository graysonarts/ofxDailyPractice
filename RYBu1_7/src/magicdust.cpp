#include "magicdust.h"

MagicDust::MagicDust(int width, int height) {

}

MagicDust::~MagicDust() {

}

void MagicDust::draw_boids() {
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetLineWidth(3.);
	for (auto& b : boids) {
		ofSetColor(b.color);
		ofDrawLine(b.pos, b.previous);
	}
}