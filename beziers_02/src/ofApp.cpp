#include "ofApp.h"

ofPolyline ofApp::getBezier(bezierPoint start, bezierPoint end) {
    
    ofPoint a = start.p;
    ofPoint b = start.p + start.a;
    ofPoint c = end.p - end.a;
    ofPoint d = end.p;
    
    ofPolyline out;
    for (float i = 0; i <= 1; i += 0.01) {
        ofPoint p = ofBezierPoint(a, b, c, d, i);
        out.addVertex(p);
    }
    
    return out.getResampledBySpacing(1);
    
}

void ofApp::getNext() {
    
    curr = next;
    
//    next.p = ofPoint(ofRandom(ofGetWidth() * 0.2, ofGetWidth() * 0.8), ofRandom(ofGetHeight() * 0.2, ofGetHeight() * 0.8));
    next.p = curr.p + ofVec2f(ofRandom(-200, 200), ofRandom(-10, 30));
    float angle = ofRandom(2 * PI);
    float radius = ofRandom(200, 300);
    next.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    nextLine = getBezier(curr, next);
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    ofBackground(254, 210, 6);
    
    gui.setup();
    gui.add(thickness.setup("thickness", 20, 1, 50));
    gui.add(timeStep.setup("timeStep", 0.1, 0, 0.5));
    
    time = 0;
    
    float angle, radius;
    
    curr.p = ofPoint(ofRandom(ofGetWidth() * 0.2, ofGetWidth() * 0.8), ofRandom(ofGetHeight() * 0.2, ofGetHeight() * 0.8));
    angle = ofRandom(2 * PI);
    radius = ofRandom(200, 300);
    curr.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    next.p = ofPoint(ofRandom(ofGetWidth() * 0.2, ofGetWidth() * 0.8), ofRandom(ofGetHeight() * 0.2, ofGetHeight() * 0.8));
    angle = ofRandom(2 * PI);
    radius = ofRandom(200, 300);
    next.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    line = getBezier(curr, next);
    getNext();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    time += timeStep;
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    line.addVertex(nextLine.getVertices()[0]);
    
    nextLine.getVertices().erase(nextLine.getVertices().begin());
    
    if (nextLine.size() == 0) getNext();
    
    if (line.size() > 5000) line.getVertices().erase(line.getVertices().begin());

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(0);
    
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofTranslate(-line.getVertices()[line.size() - 1].x, -line.getVertices()[line.size() - 1].y);
    
    line.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    line.clear();
    setup();
    
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
