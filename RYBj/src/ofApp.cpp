#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	sketch.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	font.load("Montserrat-Medium.ttf", 36);
	selectedScene = -1;
	sketches.push_back(std::make_unique<j1_2>());
	//sketches.push_back(std::make_unique<j1_3>());

	for (const auto &sketch : sketches) {
		sketch->setup();
	}

	ofAddListener(sceneChange, this, &ofApp::onSceneChange);
	onSceneChange();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (ofGetElapsedTimef() - sceneTime > sceneDuration) {
		ofNotifyEvent(sceneChange);
	}

	sketches.at(selectedScene)->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);
	ofSetColor(255);
	sketch.begin();
		sketches.at(selectedScene)->draw();
	sketch.end();

	sketch.draw(0, 0);
	sketch.readToPixels(pixels);

	ofSetColor(dominantColor(pixels));
	ofRectangle fontBox = font.getStringBoundingBox(sketchName, 0, 0);
	fontBox.x = ofGetWidth()/2. - (fontBox.width / 2.0);
	fontBox.y = ofGetHeight() - fontBox.height - labelBuffer;
	font.drawString(sketchName, fontBox.x, fontBox.y);
}

ofColor ofApp::dominantColor(ofPixels& p) {
	// Kmean cluster to 3
	// Pick dominant color
	// Calculate contrasting color for text
	return ofColor::peru;
}

void ofApp::onSceneChange() {
	sceneTime = ofGetElapsedTimef();
	selectedScene = (selectedScene + 1) % sketches.size();
	sketches.at(selectedScene)->reset();
	sketchName = sketches.at(selectedScene)->name();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofNotifyEvent(sceneChange);

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
