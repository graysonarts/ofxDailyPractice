#include "ofApp.h"
#include "ofxCsv.h"
#include "UtilsFromSo.h"

const size_t DATE_COL = 0;
const size_t MOOD_COL = 4;
const size_t COMMENT_COL = 6;
const float TILE_SIZE = 40;
const int MAX_COORD = 6;
const float STROKE_SIZE = 1.;
const glm::vec3 LABEL_OFFSET = { 3., 5., 0. };

inline glm::vec3 isoToCart(float ix, float iy, float iz = 0, float size = 1) {
	return {
		//ix * size, iy * size,
		(ix - iy) * size / 2., ((ix + iy) / 2. - iz) * size / 2.,
		0
	};
}

inline glm::vec3 CartToIso(float x, float y, float size = 1) {
	return {
		(2. * y + x) / 2., (2. * y - x) / 2., 0
	};
}

struct Date {
	int year;
	int month;
	int day;
};

Date parse_date(const string& str) {
	auto pieces = StackOverflow::split(str, '-');
	std::string::size_type sz;

	return {
		stoi(pieces.at(0), &sz),
		stoi(pieces.at(1), &sz),
		stoi(pieces.at(2), &sz)
	};
}

//--------------------------------------------------------------
void ofApp::setup() {
	load();
	background_color = ofColor::darkGray;
	stroke_color = ofColor::black;
	ofSetBackgroundColor(background_color);
	ofEnableAntiAliasing();
	month_size = isoToCart(36 / MAX_COORD, 36 % MAX_COORD, 0, TILE_SIZE) + 15.f;
	ofLog() << month_size;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < 12; i++) {
		int row = (i / 3) + 1;
		int col = (i % 3) + 1;
		float adjustment = (row % 2) * (month_size.x);
		ofPushMatrix();
		ofTranslate(month_size.x * col * 2. - adjustment, month_size.y * row);
		draw_month(i);
		//ofDrawBitmapString(ofToString(i) + "=" + ofToString(adjustment), 10, 10);
		ofPopMatrix();
	}

	//ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);
	//draw_month(10);

	//ofDrawLine(0, mouseY, ofGetWidth(), mouseY);
	//ofDrawLine(mouseX, 0, mouseX, ofGetHeight());
	//ofSetColor(0);
	//ofDrawBitmapString(ofToString(mouseX) + ", " + ofToString(mouseY), 10, 10);
}

void ofApp::draw_month(int month) {
	for (int i = 0; i < 36; i++) {
		float ix = (i / MAX_COORD);
		float iy = (i % MAX_COORD);
		draw_border_tile(ix, iy, TILE_SIZE, background_color);
	}

	for (int i = 0; i < 31; i++) {
		int mood = raw_data[month][i].mood;
		if (mood == 0) continue;

		float ix = (i / MAX_COORD);
		float iy = (i % MAX_COORD);
		float iz = (float)(mood + 3) / 8.;
		draw_tile(ix, iy, iz, TILE_SIZE, JzAzBzColor(((float)mood / 4.) + 0.5, 1.0, 1. - ((float)mood / 4.) + 0.5).color());
		//auto pt = isoToCart(ix, iy, iz, TILE_SIZE);
		//ofDrawBitmapString(ofToString(i), pt);

	}

	for (int i = 0; i < 31; i++) {
		int mood = raw_data[month][i].mood;
		const string& comment = raw_data[month][i].comment;
		if (mood != 3) continue;
		if (comment.length() == 0) continue;

		float ix = (i / MAX_COORD);
		float iy = (i % MAX_COORD);
		float iz = (float)(mood + 3) / 8.;
		draw_comment(ix, iy, iz, TILE_SIZE, raw_data[month][i].comment);
	}
}

void ofApp::draw_comment(float ix, float iy, float iz, float size, const string& comment) {
	auto center = isoToCart(ix, iy, iz, size);
	auto label_top = isoToCart(ix, iy, iz + 1.75, size);
	ofColor bg(background_color, 220);
	ofSetColor(stroke_color);
	ofSetLineWidth(STROKE_SIZE);
	ofDrawLine(center, label_top);
	ofDrawEllipse(center, STROKE_SIZE * 3., STROKE_SIZE * 2.);
	ofDrawBitmapStringHighlight(comment, label_top - LABEL_OFFSET, bg, stroke_color);
}

void ofApp::draw_tile(float ix, float iy, float iz, float size, ofColor color) {
	ofPath outline;

	outline.setStrokeColor(stroke_color);
	outline.setStrokeWidth(STROKE_SIZE);
	outline.setFillColor(color);
	outline.moveTo(isoToCart(ix - 0.5, iy - 0.5, iz, size));
	outline.lineTo(isoToCart(ix - 0.5, iy + 0.5, iz, size));
	outline.lineTo(isoToCart(ix + 0.5, iy + 0.5, iz, size));
	outline.lineTo(isoToCart(ix + 0.5, iy - 0.5, iz, size));
	outline.close();
	outline.draw();

	outline.clear();
	outline.moveTo(isoToCart(ix - 0.5, iy + 0.5, iz, size));
	outline.lineTo(isoToCart(ix + 0.5, iy + 0.5, iz, size));
	outline.lineTo(isoToCart(ix + 0.5, iy + 0.5, 0, size));
	outline.lineTo(isoToCart(ix - 0.5, iy + 0.5, 0, size));
	outline.close();
	outline.draw();

	outline.clear();
	outline.moveTo(isoToCart(ix + 0.5, iy - 0.5, iz, size));
	outline.lineTo(isoToCart(ix + 0.5, iy + 0.5, iz, size));
	outline.lineTo(isoToCart(ix + 0.5, iy + 0.5, 0, size));
	outline.lineTo(isoToCart(ix + 0.5, iy - 0.5, 0, size));
	outline.close();
	outline.draw();
}

void ofApp::draw_border_tile(float x, float y, float size, ofColor color) {
	ofPath outline;

	outline.setFilled(true);
	outline.setStrokeColor(color);
	outline.setStrokeWidth(0);
	outline.setFillColor(color);
	outline.moveTo(isoToCart(x - 0.5, y - 0.5, 0, size));
	outline.lineTo(isoToCart(x - 0.5, y + 0.5, 0, size));
	outline.lineTo(isoToCart(x + 0.5, y + 0.5, 0, size));
	outline.lineTo(isoToCart(x + 0.5, y - 0.5, 0, size));
	outline.close();
	outline.draw();

	outline.clear();
	outline.setStrokeColor(stroke_color);
	outline.setStrokeWidth(STROKE_SIZE* 2.);
	outline.setFilled(false);
	if (x == 0) {
		outline.moveTo(isoToCart(x - 0.5, y - 0.5, 0, size));
		outline.lineTo(isoToCart(x - 0.5, y + 0.5, 0, size));
	}
	if (y == 0) {
		outline.moveTo(isoToCart(x - 0.5, y - 0.5, 0, size));
		outline.lineTo(isoToCart(x + 0.5, y - 0.5, 0, size));
	}
	if (x == MAX_COORD - 1) {
		outline.moveTo(isoToCart(x + 0.5, y - 0.5, 0, size));
		outline.lineTo(isoToCart(x + 0.5, y + 0.5, 0, size));
	}

	if (y == MAX_COORD - 1) {
		outline.moveTo(isoToCart(x - 0.5, y + 0.5, 0, size));
		outline.lineTo(isoToCart(x + 0.5, y + 0.5, 0, size));
	}
	outline.draw();
}

void ofApp::drawControl(const string& label, float num, int my_mode, float& offset) {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 's': ofSaveFrame(); break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

void ofApp::increase() {
}
void ofApp::decrease() {}

void ofApp::load() {
	ofxCsv moods;
	if (!moods.load("moods.csv")) {
		ofLog(OF_LOG_ERROR) << "Unable to load moods";
		mode = -2;
		return;
	}

	moods.trim();
	bool header_seen = false;
	for (auto const& row : moods) {
		if (!header_seen) {
			header_seen = true;
			continue;
		}
		Date date = parse_date(row.getString(DATE_COL));
		string mood = row.getString(MOOD_COL);
		string comment = row.getString(COMMENT_COL);
		if (date.year != 2020) {
			continue;
		}

		if (date.day > 31) {
			ofLog() << date.month << " " << date.day << " " << mood;
		}

		MoodEntry new_mood = { Internal::mood_value(mood), comment };

		raw_data[date.month - 1][date.day - 1] = new_mood;
	}
}
void ofApp::save() {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
	ofLog() << "Window: " << w << ", " << h;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
