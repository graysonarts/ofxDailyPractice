#include "ofApp.h"

//--------------------------------------------------------------
void appRYBs21::setup(){
	seed = ofRandomf() * 65000;
	ofSeedRandom(seed);
	
	points.addVertex( -100, -100 );
	points.addVertex( 100, -100 );
	points.addVertex( 100, 100 );
	points.addVertex( -100, 100 );
	points.close();

	tornEdge = points;
	exposedEdge = points;

	length = 0.0;
}

//--------------------------------------------------------------
void appRYBs21::update(){
	float s = ofGetElapsedTimef();
	if (s - lastSplit > 1.0) {
		splitLine();
		updatePaths();
		lastSplit = s;
	}
}

//--------------------------------------------------------------
void appRYBs21::draw(){
	ofTranslate(ofGetWidth() / 2.0, ofGetHeight() / 2.0);

	ofSetColor(ofColor::gray);
	points.draw();

	int ptCount = (int)tornEdge.size();
	int idx = (int)tornEdge.getIndexAtPercent(length);
	int nxIdx = ofWrap(idx + 1, 0, ptCount);
	auto midPoint = getMidpoint(idx, nxIdx);
	ofDrawCircle(midPoint, 5);

	tornPath.setFillColor(ofColor::red);
	tornPath.draw();
}

void appRYBs21::splitLine() {
	int ptCount = (int)tornEdge.size();

	int idx = (int)tornEdge.getIndexAtPercent(length);
	int nxIdx = ofWrap(idx + 1, 0, ptCount);
	int pvIdx = ofWrap(idx - 1, 0, ptCount);
	auto newPt = getMidpoint(idx, nxIdx);

	auto offset = tornEdge.getNormalAtIndexInterpolated((float)idx + 0.5) * glm::gaussRand(5.0, 1.0);
	tornEdge.insertVertex(newPt + offset, nxIdx);
}

void appRYBs21::updatePaths() {
	updatePath(tornEdge, tornPath);
	updatePath(exposedEdge, exposedPath);
}

void appRYBs21::updatePath(ofPolyline& points, ofPath& path) {
	path.clear();
	auto vertices = points.getVertices();
	bool first = true;
	for (auto& vt : vertices) {
		if (first) {
			path.moveTo(vt);
			first = false;
		}
		else {
			path.lineTo(vt);
		}
	}
}

//--------------------------------------------------------------
void appRYBs21::keyPressed(int key){
}

glm::vec3 appRYBs21::getMidpoint(int idx, int nxIdx) {

	auto pt1 = tornEdge[idx];
	auto pt2 = tornEdge[nxIdx];

	return ((pt1 + pt2) / 2.0);
}

//--------------------------------------------------------------
void appRYBs21::keyReleased(int key){
	string filename = "frame-" + std::to_string(seed) + "-" + std::to_string(ofGetSystemTimeMillis()) + ".png";
	ofSaveViewport(filename);
}

//--------------------------------------------------------------
void appRYBs21::mouseMoved(int x, int y ){
	length = ofMap(x, 0, ofGetWidth(), 0.0, 1.0, true);
}

//--------------------------------------------------------------
void appRYBs21::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void appRYBs21::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void appRYBs21::mouseReleased(int x, int y, int button){
	splitLine();
}

//--------------------------------------------------------------
void appRYBs21::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void appRYBs21::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void appRYBs21::windowResized(int w, int h){

}

//--------------------------------------------------------------
void appRYBs21::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void appRYBs21::dragEvent(ofDragInfo dragInfo){ 

}
