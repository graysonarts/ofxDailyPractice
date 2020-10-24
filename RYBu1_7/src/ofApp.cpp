#include "ofApp.h"
#include "JzAzBzColor.h"

const int STARTING_BOIDS = 25000;
const int PALETTE_SIZE = 5;

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60.);
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	ofDisableArbTex();
	canvas.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	output.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
	tree = make_unique<ofxQuadtree>(boundary, 5.);

	char *version = (char*)glGetString(GL_VERSION);
	ofLog() << "GL " << version;

	if (!shader.load("shader")) {
		ofLog() << "Shader failed to load";
		ofExit();
	}

	paused = debug = false;
	bounds.x = ofGetWidth();
	bounds.y = ofGetHeight();
	boundary.x = ofGetWidth() / 2.;
	boundary.y = ofGetHeight() / 2.;
	boundary.width = boundary.x;
	boundary.height = boundary.y;
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
	for (int i = 0; i < cols; i++) {
		field[i] = new float[rows];
	}
	boids.reserve(STARTING_BOIDS);

	for (int i = 0; i < STARTING_BOIDS; i++) {
		ofColor c = palette.at((int)floor(ofRandomuf() * palette.size()));
		boids.push_back(new_boid_at(ofRandomWidth(), ofRandomHeight(), c));
	}

	ofSetBackgroundAuto(false);
	initialize();
}

void ofApp::initialize() {
	//canvas.begin();
	//ofBackground(32);
	//canvas.end();

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
			field[i][j] = ofNoise(xoff, yoff, zoff) * (2 * PI);
			yoff += increment;
		}
		xoff += increment;
	}

	for (auto& b : boids) {
		b.color = palette.at((int)floor(ofRandomuf() * palette.size()));
	}
}

ofColor ofApp::jab_random(float j, float a, float b) {
	j = 1;
	a += ofRandomuf();
	b += ofRandomuf();

	return jab_to_of(j, a, b);
}


//--------------------------------------------------------------
void ofApp::update(){
	if (paused) return;

	for (auto& b : boids) {
		auto& n = neighbors_of(b);

		glm::vec2 flow = field_force_at(b.pos.x, b.pos.y, glm::length(b.max_speed)) - b.vel;
		glm::vec2 sep = separation_force(b, n);
		glm::vec2 seek = avoid_obstacles(b, n);
		glm::vec2 chaos = { ofRandomf(), ofRandomf() };
		chaos = glm::normalize(chaos) * MAX_CHAOS - b.vel;
		apply_force(b, flow * 0.5 + sep + seek + chaos * 0.25);
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

	glm::vec2 force = { sin(angle), cos(angle) };

	return glm::normalize(force) * speed;
}

glm::vec2 ofApp::avoid_obstacles(boid& b, std::vector<boid*>& n) {
	glm::vec2 future = b.pos + b.vel;
	float distance = glm::length2(future);
	int count = 0;
	glm::vec2 sum = { 0, 0 };

	std::vector<boid*> possible = neighbors_of(future, NEIGHBORHOOD / 2.);
	for (auto o : possible) {
		float d = glm::distance(o->pos, b.pos);
		if (d > 0 && d < BUFFER) {
			glm::vec2 diff = o->pos - b.pos;
			sum += diff;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;
		sum *= b.max_speed;
		if (glm::length(sum) > glm::length(b.max_speed)) {
			sum = glm::normalize(sum) *  distance * b.max_speed;
		}
		sum -= b.vel;
		if (glm::length(sum) > glm::length(b.max_speed)) {
			sum = glm::normalize(sum) * b.max_speed;
		}
	}

	return { 0, 0 };
}

glm::vec2 ofApp::separation_force(boid& b, std::vector<boid*>& n) {

	int count = 0;
	glm::vec2 sum = { 0, 0 };

	for (auto o : n) {
		float d = glm::distance(o->pos, b.pos);
		if (d > 0 && d < BUFFER) {
			glm::vec2 diff = b.pos - o->pos;
			sum += diff;
			count++;
		}
	} 

	if (count > 0) {
		sum /= count;
		sum *= b.max_speed;
		if (glm::length(sum) > glm::length(b.max_speed)) {
			sum = glm::normalize(sum) * b.max_speed;
		}
		sum -= b.vel;
		if (glm::length(sum) > glm::length(b.max_speed)) {
			sum = glm::normalize(sum) * b.max_speed;
		}
	}

	return sum;
}

void ofApp::build_neighbors() {
	tree->clear();
	for (auto &b : boids) {
		tree->insert(b.pos, &b);
	}
}

std::vector<boid*> ofApp::neighbors_of(boid& b) {
	return neighbors_of(b.pos, NEIGHBORHOOD);
}

std::vector<boid*> ofApp::neighbors_of(glm::vec2& pos, float radius) {
	static std::vector<boid*> empty;

	if (tree == nullptr) {
		return empty;
	}

	size_t count = 0;

	boid** results = (boid**) tree->query(pos, radius, count);
	if (results == nullptr || count == 0) return empty;
	std::vector<boid*> retval;
	retval.reserve(count);

	for (int i = 0; i < count; i++) {
		retval.emplace_back(results[i]);
	}

	return retval;
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(32);

	canvas.begin();
	ofClear(0, 0, 0, 255);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	ofSetColor(fieldColor, 96);
	ofSetLineWidth(3.);
	for (auto& b : boids) {
		ofSetColor(b.color);
		ofDrawLine(b.pos, b.previous);
	}
	canvas.end();


	if (debug) {
		canvas.draw(0, 0);
	}
	else {
		shader.begin();
		shader.setUniform3f("resolution", { ofGetWidth(), ofGetHeight(), 0 }); //TODO: optimize
		shader.setUniformTexture("channel0", canvas.getTexture(), 0);
		shader.setUniform2f("direction", { 2., 0. }); // TODO: optimize
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		shader.end();
	}

	//canvas.getTexture().unbind();
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
	case OF_KEY_TAB:
		debug = !debug;
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
		{ 10., 10. },
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
