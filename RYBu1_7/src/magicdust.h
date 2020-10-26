#pragma once

#include "ofMain.h"
#include "ofxQuadtree.h"
#include "constants.h"
#include "boid.h"

class MagicDust {
public:
	MagicDust(int width, int height);
	~MagicDust();

	void update();
	void draw();

	// Temporarily public items
	std::vector<boid> boids; 
	void draw_boids();

private:
	std::unique_ptr<ofxQuadtree> tree;
};