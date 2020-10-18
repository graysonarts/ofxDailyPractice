#include "ofApp.h"
#include "JzAzBzColor.h"

const int STARTING_BOIDS = 1000;
const int PALETTE_SIZE = 5;

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	output.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);

	paused = false;
	bounds.x = ofGetWidth();
	bounds.y = ofGetHeight();
	rows = ofGetHeight() / SCALE;
	cols = ofGetWidth() / SCALE;
	nRows = ofGetHeight() / NEIGHBORHOOD;
	nCols = ofGetWidth() / NEIGHBORHOOD;
	increment = INCREMENT;

	palette.reserve(PALETTE_SIZE);
	for (int i = 0; i < PALETTE_SIZE; i++) {
		palette.push_back(ofColor::chartreuse);
	}

	field = new float*[cols];
	neighbors = new std::vector<boid*> * [nCols];
	for (int i = 0; i < cols; i++) {
		field[i] = new float[rows];
	}
	for (int i = 0; i < nCols; i++) {
		neighbors[i] = new std::vector<boid*>[nRows];
	}

	boids.reserve(STARTING_BOIDS);

	for (int i = 0; i < STARTING_BOIDS; i++) {
		ofColor c = palette.at((int)floor(ofRandomuf() * palette.size()));
		boids.push_back(new_boid_at(ofRandomWidth(), ofRandomHeight(), c));
	}

	//ofSetBackgroundAuto(false);
	initialize();
}

void ofApp::initialize() {
	canvas.begin();
	ofBackground(32);
	canvas.end();

	float j = ofRandomuf();
	float b = ofRandomuf();
	fieldColor = jab_to_of(0.2, j, b);
	boidColor = jab_to_of(0.8, 1 - j, 1 - b);
	neighborColor = jab_to_of(1.0, b, j);

	for (int i = 0; i < PALETTE_SIZE; i++) {
		palette[i] = jab_random(0.8, 1. - j, 1. - b);
	}

	float startY = ofRandomf();
	xoff = ofRandomf();

	for (int i = 0; i < cols; i++) {
		yoff = startY;
		for (int j = 0; j < rows; j++) {
			field[i][j] = ofNoise(xoff, yoff) * (2 * PI);
			yoff += increment;
		}
		xoff += increment;
	}

	for (auto& b : boids) {
		b.color = palette.at((int)floor(ofRandomuf() * palette.size()));
	}
}

ofColor ofApp::jab_random(float j, float a, float b) {
	j += ofRandomf();
	a += ofRandomf();
	b += ofRandomf();

	return jab_to_of(j, a, b);
}


//--------------------------------------------------------------
void ofApp::update(){
	if (paused) return;

	for (auto& b : boids) {
		// Apply Flow Field
		apply_force(b, field_force_at(b.pos.x, b.pos.y, glm::length(b.max_speed)) - b.vel);
		// Apply Spacing
		apply_force(b, separation_force(b));
	}

	for (auto& b : boids) {
		tick(0, b, bounds);
	}

	build_neighbors();
}

glm::vec2 ofApp::field_force_at(float x, float y, float speed) {
	int row = ofClamp((int)floor(y / SCALE), 0, rows - 1);
	int col = ofClamp((int)floor(x / SCALE), 0, cols - 1);
	float angle = field[col][row];

	return { speed * cos(angle), speed * sin(angle) };
}

glm::vec2 ofApp::separation_force(boid& b) {
	auto& n = neighbors_of(b);
	int count = 0;
	glm::vec2 sum = { 0, 0 };

	for (auto o : n) {
		float d = glm::distance(o->pos, b.pos);
		if (d > 0 && d < BUFFER) {
			glm::vec2 diff = glm::normalize(b.pos - o->pos);
			sum += diff;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;
		sum = glm::normalize(sum) * b.max_speed;

		sum -= b.vel;
	}

	return sum;
}

void ofApp::build_neighbors() {
	for (int col = 0; col < nCols; col++) {
		for (int row = 0; row < nRows; row++) {
			neighbors[col][row].clear();
		}
	}

	for ( boid& b : boids) {
		int x = ofClamp(floor(b.pos.x / NEIGHBORHOOD), 0, nCols - 1);
		int y = ofClamp(floor(b.pos.y / NEIGHBORHOOD), 0, nRows - 1);

		if (x < 0 || y < 0) continue;

		neighbors[x][y].push_back(&b);
	}
}

bool ofApp::is_neighbor_of(boid& subj, boid& test) {
	auto& n = neighbors_of(subj);

	auto result = std::find(n.begin(), n.end(), &test);

	return result != n.end();
}

std::vector<boid*>& ofApp::neighbors_of(boid& b) {
	static std::vector<boid*> empty;

	int x = ofClamp(floor(b.pos.x / NEIGHBORHOOD), 0, nCols - 1);
	int y = ofClamp(floor(b.pos.y / NEIGHBORHOOD), 0, nRows - 1);

	if (x < 0 || y < 0) {
		return empty;
	}

	return neighbors[x][y];
}

//--------------------------------------------------------------
void ofApp::draw(){

	canvas.begin();
	//ofBackground(32);

	//ofSetColor(fieldColor, 96);
	//ofSetLineWidth(1.);
	//glm::vec3 unit(1.f, 0.f, 0.f);
	//for (int col = 0; col < cols; col++) {
	//	for (int row = 0; row < rows; row++) {
	//		float x = (col + 1.) * SCALE;
	//		float y = (row + 1.) * SCALE;
	//		
	//		ofPushMatrix();
	//		ofTranslate(x, y);
	//		auto v = glm::rotateZ(unit, field[col][row]) * SCALE;
	//		ofDrawLine(0, 0, v.x, v.y);
	//		ofPopMatrix();


	//	}
	//}


	//ofSetLineWidth(5.);
	for (auto& b : boids) {
		ofSetColor(b.color, 10);
		ofDrawLine(b.pos, b.previous);
	}
	canvas.end();

	canvas.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 's':
		output.clear();
		canvas.readToPixels(output);
		ofSaveImage(output, "frame.png", OF_IMAGE_QUALITY_BEST);
		break;
	case ' ':
		initialize();
		break;
	case OF_KEY_RETURN:
		paused = !paused;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (button == 0) {
		boids.push_back(new_boid_at(x, y, boidColor));
	}
}

boid ofApp::new_boid_at(float x, float y, ofColor c) {
	return {
		{x, y},
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 5, 5 },
		c
	};
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
