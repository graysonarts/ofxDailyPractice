#pragma once

#include "ofMain.h"
#include "interface.h"

class j1_3 : public IDailySketch {
	const string name() const {
		return "RYBj1.2";
	}

	void draw() {
		ofBackground(ofColor::aliceBlue);
	}
};