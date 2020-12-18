#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

ofColor jab_to_of(float j, float a, float b);
glm::vec3 xyz_to_srgb(float x, float y, float z);

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

	void Jz(float j) { value[0] = j; }
	void Az(float a) { value[1] = a; }
	void Bz(float b) { value[2] = b; }

	void from(ofxXmlSettings& settings, const string &name);

	ofColor color() {
		return jab_to_of(value.x, value.y, value.z);
	}

private:
	glm::vec3 value;

	glm::vec3 to_srgb();

};


