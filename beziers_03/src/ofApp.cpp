#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60.0f);
    ofSetCurveResolution(100);
    
    _shivaVGRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
    ofSetCurrentRenderer(_shivaVGRenderer);
    
    _shivaVGRenderer->setLineJoinStyle(VG_JOIN_ROUND);
    _shivaVGRenderer->setLineCapStyle(VG_CAP_ROUND);
    
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
    
    cameraPosition = line.getVertices()[line.size() - 1];
    
}

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
    
    float angle, radius;
    
    angle = ofRandom(2 * PI);
//    angle = atan2(curr.a.y, curr.a.x);
    radius = ofRandom(100, 200);
    next.p = curr.p + ofVec2f(radius * cos(angle), radius * sin(angle));
    angle = atan2(curr.a.y, curr.a.x) + PI / 2;
    radius = ofRandom(400, 800);
    next.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    nextLine = getBezier(curr, next);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    line.addVertex(nextLine.getVertices()[0]);
    
    nextLine.getVertices().erase(nextLine.getVertices().begin());
    
    if (nextLine.size() == 0) getNext();
    
    if (line.size() > 5000) line.getVertices().erase(line.getVertices().begin());
    
    cameraPosition = 0.99 * cameraPosition + 0.01 * line.getVertices()[line.size() - 1];

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(254, 210, 6);
    
    ofSetColor(0);
    
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    //    ofTranslate(-line.getVertices()[line.size() - 1].x, -line.getVertices()[line.size() - 1].y);
    ofTranslate(-cameraPosition.x, - cameraPosition.y, 0);
    
    ofSetLineWidth(20.0f);
    line.draw();
    
    ofSetLineWidth(1.0f);
    ofSetColor(255, 0, 0);
    ofDrawCircle(curr.p, 5);
    ofSetColor(255, 0, 50);
    ofDrawCircle(curr.p + curr.a, 5);
    ofSetColor(255);
    ofDrawLine(curr.p, curr.p + curr.a);
    ofSetColor(0, 255, 0);
    ofDrawCircle(next.p, 5);
    ofSetColor(0, 255, 50);
    ofDrawCircle(next.p - next.a, 5);
    ofSetColor(255);
    ofDrawLine(next.p, next.p - next.a);
    ofSetColor(255);
    ofDrawLine(curr.p + curr.a, next.p - next.a);
    
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
