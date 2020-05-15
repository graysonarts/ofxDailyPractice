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
	virtual bool shortDuration() { return false;  }

protected:
	ofApp* parent;
};

#define SKETCH_BEGIN(sketch, color) \
class sketch : public IDailySketch { \
	public: \
		const string name() const { return #sketch ; } \
        const ofColor labelColor() const { return color; }

#define SKETCH_END };