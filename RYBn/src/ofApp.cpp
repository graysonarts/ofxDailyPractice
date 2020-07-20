#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	//ofSetLogLevel(OF_LOG_VERBOSE);

	apc.setup();

	apc.registerControl({
		ControlType::Momentary, 0, 0
		}, [&](bool b) {
			one = b;
		});

	apc.registerControl({
		ControlType::Toggle, 1, 0
		}, [&](bool b) {
			ofLog() << "Changing Color";
			two = b;
		});

	apc.registerControl(TRACK_1_KNOB,
		0.,
		[&](float f) {
			k1 = f;
		});

	apc.registerControl(TRACK_1_SLIDER,
		0.,
		[&](float f) {
			s1 = f;
		});

	apc.registerControl(TRACK_2_SLIDER,
		0.,
		[&](float f) {
			s2 = f;
		});

	ofBackground(0x333333);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofTranslate(ofGetWidth() / 2., ofGetHeight() / 2.);

	ofSetColor(two ? ofColor::antiqueWhite : ofColor::crimson);

	ofFill();
	if (one) {
		ofDrawRectangle(-25., -25, (s1 + 40.), (s2 + 40.));
	}

	ofNoFill();
	ofDrawEllipse(0, 0, k1 + 90., k1 + 90.);
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