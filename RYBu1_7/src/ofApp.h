#pragma once

#include "ofMain.h" 
#include "constants.h"
#include "ofxQuadtree.h"
#include "boid.h"
#include "magicdust.h"

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
	int cols, rows, nRows, nCols;
	float xoff, yoff, zoff;
	float increment;
	bool paused, debug;
	ofFbo channel0, channel1;
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
	void draw_with(ofFbo& source, ofFbo& target, ofShader& shader, const glm::vec2& direction, float scale);
	

	ofRectangle boundary;

	ofShader blur_shade;
	ofShader add_shade;

	std::unique_ptr<MagicDust> dust;
	std::unique_ptr<ofxQuadtree> tree;
};
