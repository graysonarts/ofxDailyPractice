#pragma once

#include "ofMain.h" 
#include "ofxkdtree.h"
#include "boid.h"

const float SCALE = 30;
const float NEIGHBORHOOD = 50;
const float BUFFER = 50;
const float INCREMENT = 0.05;
const float MAX_CHAOS = 10.;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
	std::vector<ofColor> palette;
	ofColor fieldColor;
	ofColor boidColor;
	ofColor neighborColor;
	float** field;
	std::vector<boid> boids;
	int cols, rows, nRows, nCols;
	float xoff, yoff, zoff;
	float increment;
	bool paused, debug;
	ofFbo canvas;
	ofPixels output;
	ofColor jab_random(float j, float a, float b);

	void initialize();
	glm::vec2 field_force_at(float x, float y, float speed);

	glm::vec2 bounds;
	boid new_boid_at(float x, float y, ofColor c);
	void build_neighbors();
	std::vector<boid*> neighbors_of(boid& b);
	std::vector<boid*> neighbors_of(glm::vec2& pt, float r);
	glm::vec2 separation_force(boid& b, std::vector<boid*>& n);
	glm::vec2 avoid_obstacles(boid& b, std::vector<boid*>& n);
	
	std::unique_ptr<ofxQuadtree> tree;
	ofRectangle boundary;

	ofShader shader;
};
