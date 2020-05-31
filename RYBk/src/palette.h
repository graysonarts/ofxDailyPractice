#pragma once

#include "ofMain.h"

class PaletteSource {
	public:
		PaletteSource(string path);
		bool switchSource(string path);
		ofColor getColorAt(const glm::ivec2 position) const;
		ofColor getColorAt(const int x, const int y) const;
		glm::ivec2 numColors() const;

	private:
		ofImage *source;

};