#pragma once

#include "ofMain.h"
#include "interface.h"

class j1_3 : public IDailySketch {
	const string name() const {
		return "j1.3";
	}

	void draw() {
		ofBackground(ofColor::aliceBlue);
	}
};