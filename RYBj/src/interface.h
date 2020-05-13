#pragma once

class IDailySketch {
public:
	virtual const string name() const = 0;
	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void reset() {}
};