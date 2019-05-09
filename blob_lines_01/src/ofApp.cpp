#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFullscreen(true);
    ofHideCursor();
    
    ofBackground(0,0,0);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    
    int count = 400;
    for (int i = 0; i < count; i++){
        particle myParticle;
        
        float x = 500 + 100 * cos ( (i / (float)count) * TWO_PI);
        float y = 500 + 100 * sin ( (i / (float)count) * TWO_PI);
        
        myParticle.setInitialCondition(x,y ,0,0);
        particles.push_back(myParticle);
    }
    
    for (int i = 0; i < particles.size(); i++) {
        spring mySpring;
        mySpring.distance        = 1;
        mySpring.springiness    = 0.2f;
        mySpring.particleA = & (particles[i  ]);
        mySpring.particleB = & (particles[(i+1) % particles.size()]);
        springs.push_back(mySpring);
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    t = 0.25 * ofGetElapsedTimef();
    w = ofGetWidth();
    h = ofGetHeight();
    
    for (int i = 0; i < particles.size(); i++){
        particles[i].resetForce();
    }
    
    float noiseRes = 1000;
    float offsetDist = 100;
    
    for (int i = 0; i < particles.size(); i++){
        
        particles[i].addAttractionForce(w / 2.0, h / 2.0, 100000, 0.1f);
        
//        float dist = offsetDist * ofNoise(particles[i].pos.x / noiseRes, particles[i].pos.y / noiseRes, 1000, t);
//        float angle = 4 * PI * ofNoise(particles[i].pos.x / noiseRes, particles[i].pos.y / noiseRes, 2000, t);
        float dist = offsetDist * ofNoise(particles[i].ogPos.x / noiseRes, particles[i].ogPos.y / noiseRes, 1000, t);
        float angle = 4 * PI * ofNoise(particles[i].ogPos.x / noiseRes, particles[i].ogPos.y / noiseRes, 2000, t);
        float dx = dist * cos(angle);
        float dy = dist * sin(angle);
        particles[i].addRepulsionForce(particles[i].pos.x + dx, particles[i].pos.y + dy, offsetDist, 0.5f);
        
        for (int j = 0; j < i; j++){
            particles[i].addRepulsionForce(particles[j], 20, 0.1);
        }
    }
    
    for (int i = 0; i < springs.size(); i++){
        springs[i].update();
    }
    
    float xAvg = 0;
    float yAvg = 0;
    for (int i = 0; i < particles.size(); i++){
        particles[i].addDampingForce();
        particles[i].update();
        xAvg += particles[i].pos.x;
        yAvg += particles[i].pos.y;
    }
    xAvg /= particles.size();
    yAvg /= particles.size();
    
    ofVec2f diff(ofGetWidth() / 2 - xAvg, ofGetHeight() / 2 - yAvg);
    for (int i = 0; i < particles.size(); i++){
        particles[i].pos += diff;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255, 255);
    
    ofPolyline tmp;
    for (int i = 0; i < particles.size(); i++){
        tmp.addVertex(particles[i].pos.x, particles[i].pos.y);
    }
    tmp.close();
    
    ofRectangle bb = tmp.getBoundingBox();
    float scale = (MIN(w, h) - 300) / MAX(bb.getWidth(), bb.getHeight());
    
    tmp.translate(glm::vec2(-w / 2.0, -h / 2.0));
    tmp.scale(scale, scale);
    tmp.translate(glm::vec2(w / 2.0, h / 2.0));
    
    tmp.getSmoothed(5).draw();
    ofPolyline resamp = tmp.getSmoothed(5).getResampledByCount(601);
    
//    tmp.draw();
//    ofPolyline resamp = tmp;
    
    for (int i = 0; i < resamp.size(); i++) {
        float angle = ofMap(i, 0, resamp.size(), 0, 2 * PI);
//        float angle = ofMap(mouseX, 0, w, 0, 2 * PI);
        ofVec3f dir = ofVec3f(cos(angle), sin(angle), 0);
//        ofVec3f dir = resamp.getVertices()[i] - ofVec3f(w / 2, h / 2, 0);
        ofVec2f a = resamp.getVertices()[i];
        ofVec2f b = resamp.getVertices()[i] + 250 * dir.getNormalized();

        for (int j = 0; j < resamp.size(); j++) {
            if (j != i && j != i - 1 && !(i == 0 && j == resamp.size() - 1)) {
                ofVec2f c = resamp.getVertices()[j];
                ofVec2f d = resamp.getVertices()[(j + 1) % resamp.size()];

                ofVec2f intersect(0, 0);
                if (ofLineSegmentIntersection(a, b, c, d, intersect)) {
                    b = intersect;
                }
            }
        }

        ofMesh tmpMesh;
        tmpMesh.setMode(OF_PRIMITIVE_LINES);
        tmpMesh.addVertex(ofVec3f(a.x, a.y, 0));
        tmpMesh.addColor(ofColor(255, 255));
        tmpMesh.addVertex(ofVec3f(b.x, b.y, 0));
        tmpMesh.addColor(ofColor(0, 0));

        tmpMesh.draw();
    }
    

    
    
    
//    ofMesh tmpMesh;
//    tmpMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//    for (int i = 0; i <= resamp.size(); i++) {
////        float angle = ofMap(i, 0, resamp.size(), 0, 2 * PI);
//        float angle = ofMap(mouseX, 0, w, 0, 2 * PI);
//        ofVec3f dir = ofVec3f(200 * cos(angle), 200 * sin(angle), 0);
//        ofVec2f a = resamp.getVertices()[i % resamp.size()];
//        ofVec2f b = resamp.getVertices()[i % resamp.size()] + dir;
//
//        for (int j = 0; j < resamp.size(); j++) {
//            if (j != i && j != i - 1) {
//                ofVec2f c = resamp.getVertices()[j % resamp.size()];
//                ofVec2f d = resamp.getVertices()[(j + 1) % resamp.size()];
//
//                ofVec2f intersect(0, 0);
//                if (ofLineSegmentIntersection(a, b, c, d, intersect)) {
//                    b = intersect;
//                }
//            }
//        }
//
//        tmpMesh.addVertex(ofVec3f(a.x, a.y, 0));
//        tmpMesh.addColor(ofColor(255, 255));
//        tmpMesh.addVertex(ofVec3f(b.x, b.y, 0));
//        tmpMesh.addColor(ofColor(0, 0));
//
//    }
//    tmpMesh.draw();

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
