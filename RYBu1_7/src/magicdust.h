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
	void build_neighbors();
	std::vector<boid*> neighbors_of(boid& b);
	std::vector<boid*> neighbors_of(glm::vec2& pt, float r);

private:
	ofRectangle boundary;
	glm::vec2 bounds;
	ofxQuadtree tree;

};