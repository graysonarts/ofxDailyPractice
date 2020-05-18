#pragma once

#include "ofMain.h"

class ofApp;

class IDailySketch {
public:
	IDailySketch(ofApp* parent) : parent(parent) { }
	virtual const string name() const = 0;
	virtual const ofColor labelColor() const = 0;
	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void reset() {}
	virtual void stop() {}
	virtual bool shortDuration() { return false;  }
	virtual ofShader* shader() { return nullptr;  }
	virtual void onButton(float x, float y) { }
	virtual void onMove(float x, float y) { }
	virtual bool isInteractive() { return false; }

protected:
	ofApp* parent;
};

#define SKETCH_BEGIN(sketch, color) \
class sketch : public IDailySketch { \
	string _name; \
	public: \
		sketch(ofApp* parent) : IDailySketch(parent) { \
			_name = #sketch; \
			_name.replace(2, 1, "."); \
		} \
		const string name() const { return _name ; } \
        const ofColor labelColor() const { return color; }

#define SKETCH_END };

//#define NDI_OUTPUT

#ifdef NDI_OUTPUT
#include "ofxNDI.h"

class NdiOut {
public:
	NdiOut(unsigned int width, unsigned int height) : width(width), height(height) {
		strcpy_s(senderName, 256, "RYB cycle");
		ofSetWindowTitle(senderName);

		sender.SetReadback();
		sender.SetAsync();
		sender.CreateSender(senderName, width, height);
	}
	~NdiOut() {
		sender.ReleaseSender();
	}

	void getFrame(ofFbo& img) {
		sender.SendImage(img);

		//img.begin();
		//if (sender.SenderCreated()) {
		//	char str[256];
		//	sprintf_s(str, 256, "Sending as : [%s] (%dx%d)", senderName, width, height);
		//	ofDrawBitmapString(str, 20, 30);
		//	// Show fps
		//	sprintf_s(str, 256, "fps: %3.3d (%4.2f)", (int)ofGetFrameRate(), sender.GetFps());
		//	ofDrawBitmapString(str, ofGetWidth() - 140, 30);
		//}
		//img.end();
	}

private:
	unsigned int width, height;
	ofxNDIsender sender;
	char senderName[256];
};
#endif // NDI_OUTPUT