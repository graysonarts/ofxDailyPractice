#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class appRYBs21 : public ofBaseApp{

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
		ofPolyline points;
		ofPolyline exposedEdge;
		ofPolyline tornEdge;
		ofPath exposedPath;
		ofPath tornPath;
		float length;
		int seed;

		void splitLine();
		glm::vec3 getMidpoint(int idx, int nxIdx);
		void updatePaths();
		void updatePath(ofPolyline& points, ofPath& path);
		float lastSplit;
		
};

class appRYBs23 : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void draw_section();

private:
	int seed;
	ofParameter<float> main_size;
	ofParameter<float> strip_width;
	ofParameter<float> offset;
	float height;
	ofPath triangle;
	ofFbo mask;
	ofFbo drawing;

	ofxPanel gui;
};