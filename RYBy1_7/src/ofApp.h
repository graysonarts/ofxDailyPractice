#pragma once

#include "ofMain.h"
#include "JzAzBzColor.h"
#include "ofxXmlSettings.h"

struct MoodEntry {
	int mood;
	string comment;

	MoodEntry() : mood(0), comment("") { }
	MoodEntry(int mood, string comment) : mood(mood), comment(comment) { }
	MoodEntry(MoodEntry const&) = default;
};

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
  

		void drawControl(const string& label, float num, int my_mode, float& offset);
		void increase();
		void decrease();
		void save();
		void load();
		void draw_month(int month);
		void draw_tile(float x, float y, float z, float size, ofColor color);
		void draw_border_tile(float x, float y, float size, ofColor color);
		void draw_comment(float x, float y, float z, float size, const string& comment);

		int mode = -1;
		glm::vec3 month_size;

		MoodEntry raw_data[12][31];
		ofColor stroke_color;
		ofColor background_color;
};
