#pragma once

#include "ofMain.h"

class CIEColor
{
public:
	CIEColor(ofColor color) : CIEColor(color.r, color.g, color.b, color.limit()) {

		
	}

	CIEColor(float r, float g, float b, float limit) : CIEColor(r/limit, g/limit, b/limit) {

	}

	CIEColor(float r, float g, float b) {
		glm::vec3 rgb(r, g, b);
		value = rgb_to_cie(rgb);
	}

	CIEColor(glm::vec3& cie) {
		value = cie;
	}

	const float x() { return value[0]; }
	const float y() { return value[1]; }
	const float z() { return value[2]; }

	ofColor color() {
		glm::vec3 rgb = cie_to_rgb(value);

		rgb *= 255;

		return ofColor(rgb[0], rgb[1], rgb[2]);
	}


private:
	glm::vec3 value;

	float gamma(float x) {
		return (x > 0.0031308)
			? 1.055 * pow(x, 0.4166667) - 0.055
			: 12.92 * x;
	}

	float gamma_inverse(float x) {
		return (x > 0.04045)
			? pow((x + 0.055) / 1.055, 2.4)
			: x / 12.92;
	}

	glm::vec3 cie_to_rgb(glm::vec3& cie) {
		static const glm::mat3 factorPrime = glm::inverse(factor);

		glm::vec3 rgb = factorPrime * cie;
		rgb.r = gamma(rgb.r);
		rgb.g = gamma(rgb.g);
		rgb.b = gamma(rgb.b);

		return rgb;
	}

	glm::vec3 rgb_to_cie(glm::vec3& rgb) {
		glm::vec3 gamma_rgb(
			gamma_inverse(rgb[0]),
			gamma_inverse(rgb[1]),
			gamma_inverse(rgb[2]));

		return factor * gamma_rgb;
	}

	static const glm::mat3 factor;
};


