#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	m_app.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	m_app.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	m_app.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case ' ':
			m_app.reset();
			break;
		case 's':
			ofSaveFrame();
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
	m_app.set_padding_from_window_size(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
