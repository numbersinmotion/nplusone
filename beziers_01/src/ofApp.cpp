#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    ofBackground(254, 210, 6);
    
    gui.setup();
    gui.add(thickness.setup("thickness", 20, 1, 50));
    gui.add(timeStep.setup("timeStep", 0.01, 0, 0.1));
    
    time = 0;
    
    for (int i = 0; i < 10; i++) {
        
        bezierPoint tmp;
        
        tmp.p = ofPoint(ofRandom(ofGetWidth() * 0.2, ofGetWidth() * 0.8), ofRandom(ofGetHeight() * 0.2, ofGetHeight() * 0.8));
        float angle = ofRandom(2 * PI);
        float radius = ofRandom(100, 200);
        tmp.a = ofPoint(radius * cos(angle), radius * sin(angle));
        
        bezierPoints.push_back(tmp);
        
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    time += timeStep;
    
    for (int i = 0; i < bezierPoints.size(); i++) {
        bezierPoints[i].a.rotate(0, 0, timeStep);
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPolyline line;
    
    for (int i = 0; i < bezierPoints.size(); i++) {
        
        bezierPoint curr = bezierPoints[i];
        bezierPoint next = bezierPoints[(i + 1) % bezierPoints.size()];
        
        // corner points
        ofPoint a = curr.p;
        ofPoint d = next.p;
        
        // anchor points
        ofPoint b = a + curr.a;
        ofPoint c = d - next.a;
        
        for (float j = 0; j <= 1; j += 0.01) {
            ofPoint p = ofBezierPoint(a, b, c, d, j);
            line.addVertex(p);
        }
        
    }
    line.close();
    
    line = line.getResampledBySpacing(1);
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for (int i = 0; i < line.size(); i++) {
        
        ofVec2f curr = line[i];
        ofVec2f prev, next;
        if (i == 0) {
            prev = line[line.size() - 1];
            next = line[i + 1];
        } else if (i == line.size() - 1) {
            prev = line[i - 1];
            next = line[0];
        } else {
            prev = line[i - 1];
            next = line[i + 1];
        }
        ofVec2f dir = prev - next;
        dir.normalize();
        dir *= thickness;
        dir.rotate(90);
        
        mesh.addVertex(glm::vec3(curr.x, curr.y, 0) + glm::vec3(dir.x, dir.y, 0));
        mesh.addColor(ofColor(0));
        mesh.addVertex(glm::vec3(curr.x, curr.y, 0) - glm::vec3(dir.x, dir.y, 0));
        mesh.addColor(ofColor(0));
        
    }
    mesh.draw();
    
    line.draw();
    
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
