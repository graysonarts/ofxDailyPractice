#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
	gui.add(z.set("Zed", 0.2, 0., 1.));
	gui.add(increment.set("noise step", 0.01, 0., 1.));

	z = 0.2;
	offset = { 0., 0., 0. };
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	for (int y = 0; y < ofGetHeight(); y+=5) {
		for (int x = 0; x < ofGetWidth(); x+=5) {
			float noise = ofNoise(offset.x, offset.y, offset.z);
			float xn = ofNormalize(x, 0, ofGetWidth());
			float yn = ofNormalize(y, 0, ofGetHeight());
			JzAzBzColor c(
				noise,
				z * noise,
				noise * sin(ofGetElapsedTimef()));
			ofSetColor(c.color());
			ofDrawCircle(x, y, 5);

			offset.x += increment;
		}
		offset += { -offset.x, increment, 0. };
	}

	offset = { 0, 0, offset.z + increment };

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//z = ofMap(x, 0, ofGetWidth(), 0.1, 0.9);
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
