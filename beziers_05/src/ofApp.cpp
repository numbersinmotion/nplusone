#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    debugMode = false;
    
    followIndex = 0;
    
    ofSetFrameRate(30.0f);
    ofSetCurveResolution(100);
    
    _shivaVGRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
    ofSetCurrentRenderer(_shivaVGRenderer);
    _shivaVGRenderer->setLineJoinStyle(VG_JOIN_ROUND);
    _shivaVGRenderer->setLineCapStyle(VG_CAP_ROUND);
    
    setupWalkers();
    
}

void ofApp::setupWalkers() {
    
    // random walkers
//    int countWalkers = 5;
//    for (int i = 0; i < countWalkers; i++) {
//        BezierWalker tmpWalker;
//        tmpWalker.setup(ofMap(i, 0, countWalkers - 1, 255, 0), ofMap(i, 0, countWalkers - 1, 3, 100));
//        walkers.push_back(tmpWalker);
//    }
    
    // trying to match cover
    BezierWalker thinWhite;
    thinWhite.setup(ofColor(255), 1, 3, 50, 100);
    walkers.push_back(thinWhite);
    thinWhite.setup(ofColor(255), 1, 3, 50, 100);
    walkers.push_back(thinWhite);
    thinWhite.setup(ofColor(255), 1, 3, 50, 100);
    walkers.push_back(thinWhite);
    BezierWalker thickWhite;
    thickWhite.setup(ofColor(255), 50, 1, 300, 500);
    walkers.push_back(thickWhite);
    thickWhite.setup(ofColor(255), 50, 1, 300, 500);
    walkers.push_back(thickWhite);
    BezierWalker mediumBlack;
    mediumBlack.setup(ofColor(0), 25, 2, 150, 250);
    walkers.push_back(mediumBlack);
    mediumBlack.setup(ofColor(0), 25, 2, 150, 250);
    walkers.push_back(mediumBlack);
    BezierWalker thickBlack;
    thickBlack.setup(ofColor(0), 50, 1, 300, 500);
    walkers.push_back(thickBlack);
    thickBlack.setup(ofColor(0), 50, 1, 300, 500);
    walkers.push_back(thickBlack);
    thickBlack.setup(ofColor(0), 50, 1, 300, 500);
    walkers.push_back(thickBlack);
    thickBlack.setup(ofColor(0), 50, 1, 300, 500);
    walkers.push_back(thickBlack);
    
    // follow avg position
        cameraPosition = ofVec2f(0, 0);
        for (int i = 0; i < walkers.size(); i++) {
            cameraPosition += walkers[i].line.getVertices()[walkers[i].line.size() - 1];
        }
        cameraPosition /= walkers.size();
    
    // follow random single walker
//    cameraPosition = walkers[followIndex].line.getVertices()[walkers[followIndex].line.size() - 1];
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    for (int i = 0; i < walkers.size(); i++) {
        walkers[i].update();
    }
    
    // follow avg position
    ofVec2f newCameraPosition = ofVec2f(0, 0);
    for (int i = 0; i < walkers.size(); i++) {
        newCameraPosition += walkers[i].line.getVertices()[walkers[i].line.size() - 1];
    }
    newCameraPosition /= walkers.size();
    cameraPosition = 0.99 * cameraPosition + 0.01 * newCameraPosition;
    
    // follow random single walker
//    if ((ofGetFrameNum() + 1) % 400 == 0) followIndex = floor(ofRandom(walkers.size()));
//    cameraPosition = 0.99 * cameraPosition + 0.01 * walkers[followIndex].line.getVertices()[walkers[followIndex].line.size() - 1];

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(254, 210, 6);
    
    ofSetColor(0);
    
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofTranslate(-cameraPosition.x, - cameraPosition.y, 0);
    
    ofSetLineWidth(20.0f);
    for (int i = 0; i < walkers.size(); i++) {
        walkers[walkers.size() - 1 - i].draw();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'r' || key == 'R') {
        for (int i = 0; i < walkers.size(); i++) {
            walkers.clear();
        }
        setupWalkers();
    } else if (key == ' ') {
        debugMode = !debugMode;
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
