#pragma once

#include "ofMain.h"

enum Edges {
	A = 0b0000000000000001,
	B = 0b0000000000000010,
	C = 0b0000000000000100,
	D = 0b0000000000001000,
	E = 0b0000000000010000,
	F = 0b0000000000100000,
	G = 0b0000000001000000,
	H = 0b0000000010000000,
	I = 0b0000000100000000,
	J = 0b0000001000000000,
	K = 0b0000010000000000,
	L = 0b0000100000000000,
};

struct Cell {
	string path;
	std::unique_ptr<ofImage> image;
	Edges edges;

	bool satisfies(Edges test) {
		return (edges & test) == edges;
	}
};