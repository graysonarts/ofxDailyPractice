#include "magicdust.h"

MagicDust::MagicDust(int width, int height) : boundary({ width / 2.f, height / 2.f, width / 2.f, height / 2.f}), bounds(width, height), tree(boundary, 10.) {

}

MagicDust::~MagicDust() {

}

void MagicDust::update() {
	for (auto& b : boids) {
		tick(0, b, bounds);
	}

	build_neighbors();
}

void MagicDust::draw_boids() {
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetLineWidth(3.);
	for (auto& b : boids) {
		ofSetColor(b.color);
		ofDrawLine(b.pos, b.previous);
	}
}

void MagicDust::build_neighbors() {
	tree.clear();
	for (auto& b : boids) {
		tree.insert(b.pos, &b);
	}
}

std::vector<boid*> MagicDust::neighbors_of(glm::vec2& pos, float radius) {
	static std::vector<boid*> empty;

	size_t count = 0;

	boid** results = (boid**)tree.query(pos, radius, count);
	if (results == nullptr || count == 0) return empty;
	std::vector<boid*> retval;
	retval.reserve(count);

	for (int i = 0; i < count; i++) {
		retval.emplace_back(results[i]);
	}

	return retval;
}

std::vector<boid*> MagicDust::neighbors_of(boid& b) {
	return neighbors_of(b.pos, NEIGHBORHOOD);
}