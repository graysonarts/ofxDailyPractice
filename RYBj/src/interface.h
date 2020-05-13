#pragma once

class IDailySketch {
public:
	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void reset() {}
};