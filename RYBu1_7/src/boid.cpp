#include "boid.h"

const float MAX_FORCE = 100.;

void apply_force(boid& b, const glm::vec2& force) {
	b.accel += force;
	auto l = glm::length(b.accel);
	if (l > MAX_FORCE) {
		b.accel = glm::normalize(b.accel) * MAX_FORCE;
	}
}

void tick(const float dt, boid &b, const glm::vec2& boundaries) {
	b.previous = b.pos;
	b.vel += b.accel;
	b.vel = glm::clamp(b.vel, -b.max_speed, b.max_speed);
	b.pos += b.vel;

	b.accel *= 0.;

	if (b.pos.x < 0) {
		b.pos.x = b.previous.x = boundaries.x;
	}
	if (b.pos.x > boundaries.x) {
		b.pos.x = b.previous.x = 0;
	}

	if (b.pos.y < 0) { b.pos.y = b.previous.y = boundaries.y; }
	if (b.pos.y > boundaries.y) { b.pos.y = b.previous.y = 0; }
}