#pragma once

#include "j1_2.h"
// j1_3 - SHAME!
#include "j1_4.h"
#include "j1_5.h"
#include "shame.h"

#define SKETCH(sketch) retval.push_back(std::make_unique<sketch>(parent))
#define SHAME(name, background, label) retval.push_back(std::make_unique<shame>(parent, #name, background, label))

std::vector<std::unique_ptr<IDailySketch> > getSketches(ofApp* parent) {
	std::vector<std::unique_ptr<IDailySketch> > retval;

	SKETCH(j1_4);
	SHAME(j1_3, ofColor::aliceBlue, ofColor::darkBlue);
	SKETCH(j1_2);

	return retval;
}