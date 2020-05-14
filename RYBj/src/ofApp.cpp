#include "ofApp.h"
#include "sketches/j1_2.h"
#include "sketches/j1_3.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
	gui.add(labelBuffer.set("Label Bottom", 30., 0., ofGetHeight()));
	gui.add(lineBuffer.set("Line Spacing", 0., -100., 100.));

	gui.loadFromFile("settings.xml");


	sketch.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	light.load("fonts/Montserrat-Light.ttf", 24);
	semibold.load("fonts/Montserrat-SemiBold.ttf", 8);
	selectedScene = -1;
	sketches.push_back(std::make_unique<j1_3>(this));
	sketches.push_back(std::make_unique<j1_2>(this));


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

	ofSetColor(labelColor);
	ofRectangle fontBox = light.getStringBoundingBox(sketchName, 0, 0);
	fontBox.x = ofGetWidth()/2. - (fontBox.width / 2.0);
	fontBox.y = ofGetHeight() - fontBox.height - labelBuffer;
	light.drawString(sketchName, fontBox.x, fontBox.y);

	fontBox = semibold.getStringBoundingBox("CYCLE RYB", 0, fontBox.y + fontBox.height + lineBuffer);
	fontBox.x = ofGetWidth() / 2. - (fontBox.width / 2.0);
	semibold.drawString("CYCLE RYB", fontBox.x, fontBox.y);

	if (showGui) gui.draw();
}

void ofApp::onSceneChange() {
	sceneTime = ofGetElapsedTimef();
	selectedScene = (selectedScene + 1) % sketches.size();
	sketches.at(selectedScene)->reset();
	sketchName = "sk. " + sketches.at(selectedScene)->name();
	labelColor = sketches.at(selectedScene)->labelColor();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case ' ':
		ofNotifyEvent(sceneChange);
		break;
	case OF_KEY_BACKSPACE:
		showGui = !showGui;
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
