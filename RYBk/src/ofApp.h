#pragma once

#include "ofMain.h"
#include "interface.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"

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
		void onSceneChange();

		ofEvent<void> sceneChange;
		ofParameter<float> sceneDuration = 120.;
		ofParameter<float> shortSceneDuration = 30.;
		ofParameter<float> labelBuffer = 30.;
		ofParameter<float> lineBuffer = 0.;

		std::vector<std::unique_ptr<IDailySketch> > sketches;
		float sceneTime;
		size_t selectedScene;
		ofColor labelColor;
		string sketchName;
		ofFbo sketch;
		ofPixels pixels;
		ofTrueTypeFont light, semibold;

		ofxPanel gui;
		bool showGui = false;
		ofImage savedScreen;
		
#ifdef NDI_OUTPUT
		NdiOut *ndi;
#endif
};
