#pragma once

#include "ofMain.h"
#include "CIEColor.h"

class JzAzBzColor
{
public:
	JzAzBzColor(ofColor color) : JzAzBzColor(color.r, color.g, color.b) {
	}

	JzAzBzColor(float j, float a, float b) : value({ j, a, b }) {
	}

	const float Jz() { return value[0]; }
	const float Az() { return value[1]; }
	const float Bz() { return value[2]; }

	ofColor color() {
		glm::vec3 srgb = to_srgb() * 255.0;
		srgb = glm::clamp(srgb, { 0., 0., 0. }, { 255., 255., 255. });
		return { (float)srgb[0], (float)srgb[1], (float)srgb[2] };
	}

private:
	glm::vec3 value;

	glm::vec3 to_srgb();

};

