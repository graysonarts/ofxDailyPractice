#include "ofApp.h"

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
	ofRectangle fontBox = light.getStringBoundingBox(sketchName, 0, 0);
	fontBox.x = ofGetWidth()/2. - (fontBox.width / 2.0);
	fontBox.y = ofGetHeight() - fontBox.height - labelBuffer;
	light.drawString(sketchName, fontBox.x, fontBox.y);

	fontBox = semibold.getStringBoundingBox("CYCLE RYB", 0, fontBox.y + fontBox.height + lineBuffer);
	fontBox.x = ofGetWidth() / 2. - (fontBox.width / 2.0);
	semibold.drawString("CYCLE RYB", fontBox.x, fontBox.y);

	gui.draw();
}

ofColor ofApp::dominantColor(ofPixels& p) {

	return ofColor::peru;

	// Prep the image
	ofxCvColorImage image;
	image.allocate(p.getWidth(), p.getHeight());
	image.setFromPixels(p);
	image.resize(108, 108);
	//image.blur(15);

	// Calculate KMean(3)
	cv::Mat data = cv::cvarrToMat(image.getCvImage(), true); // Copies data
	data = data.reshape(1, data.rows * data.cols);
	data.convertTo(data, CV_32FC3);
	cv::Mat labels, centers;

	try {
		cv::kmeans(data, 3, labels, cv::TermCriteria(CV_TERMCRIT_ITER, 10, 1.0), 1, cv::KMEANS_PP_CENTERS, centers);
	}
	catch (cv::Exception e) {
		ofLog(OF_LOG_ERROR) << e.what();
		return ofColor::chartreuse;
	}

	centers.forEach<float>([](float& p, const int *position) {
		ofLog() << p << ": " << position[0] << ", " << position[1];
	});

	// Kmean cluster to 3
	// Pick dominant color
	// Calculate contrasting color for text
	return ofColor::peru;
}

void ofApp::onSceneChange() {
	sceneTime = ofGetElapsedTimef();
	selectedScene = (selectedScene + 1) % sketches.size();
	sketches.at(selectedScene)->reset();
	sketchName = "sk. " + sketches.at(selectedScene)->name();
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
