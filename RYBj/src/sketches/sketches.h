#pragma once

#include "j1_2.h"
// j1_3 - SHAME!
#include "j1_4.h"
#include "j1_5.h"
#include "j1_6.h"
#include "j1_7.h"
#include "j2_1.h"
#include "j2_2.h"
#include "j2_3.h"
#include "j2_4.h"
#include "j2_5.h"
#include "k1_6.h"
/// INCLUDE






#include "shame.h"

#define SKETCH(sketch) retval.push_back(std::make_unique<sketch>(parent))
#define SHAME(name, background, label) { \
 string _name = #name; \
_name.replace(2, 1, "."); \
 retval.push_back(std::make_unique<shame>(parent, _name, background, label)); \
}

std::vector<std::unique_ptr<IDailySketch> > getSketches(ofApp* parent) {
	std::vector<std::unique_ptr<IDailySketch> > retval;

	/// SKETCH LIST
	SKETCH(k1_6);
	SKETCH(j2_5);
	SKETCH(j2_4);
	SKETCH(j2_3);
	SKETCH(j2_2);
	SKETCH(j2_1);
	SKETCH(j1_7);
	SKETCH(j1_6);
	SKETCH(j1_5);
	SKETCH(j1_4);
	SHAME(j1_3, ofColor::aliceBlue, ofColor::darkBlue);
	SKETCH(j1_2);

	return retval;
}