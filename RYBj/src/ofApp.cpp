#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	selectedScene = 0;
	sketches.push_back(std::make_unique<j1_2>());
	//sketches.push_back(std::make_unique<j1_3>());

	for (const auto &sketch : sketches) {
		sketch->setup();
	}

	ofAddListener(sceneChange, this, &ofApp::onSceneChange);
	sketches.at(selectedScene)->reset();
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
	sketches.at(selectedScene)->draw();

}

void ofApp::onSceneChange() {
	sceneTime = ofGetElapsedTimef();
	selectedScene = (selectedScene + 1) % sketches.size();
	sketches.at(selectedScene)->reset();
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
