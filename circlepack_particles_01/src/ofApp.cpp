#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFullscreen(true);
    
    // this number describes how many bins are used
    // on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
    // if this number is too high, binning is not effective
    // because the screen is not subdivided enough. if
    // it's too low, the bins take up so much memory as to
    // become inefficient.
    int binPower = 3;
    
    particleSystem.clearParticles();
    particleSystem.setup(ofGetWidth(), ofGetHeight(), binPower);
    
    kParticles = 20;
    float padding = 0;
    float maxVelocity = .5;
    for(int i = 0; i < kParticles * 1024; i++) {
        float x = ofRandom(0, ofGetWidth());
        float y = ofRandom(padding, ofGetHeight() - padding);
        float xv = ofRandom(-maxVelocity, maxVelocity);
        float yv = ofRandom(-maxVelocity, maxVelocity);
        Particle particle(x, y, xv, yv);
        particleSystem.add(particle);
    }
    
    ofBackground(0, 0, 0);
    
//    timeStep = time;
    lineOpacity = 128;
    pointOpacity = 255;
    isMousePressed = false;
    slowMotion = false;
    particleNeighborhood = 4;
//    particleRepulsion = 2;
//    centerAttraction = .01;
    
    getForces();
    
    timeStep = 1;
    particleRepulsion = 15;
    textForce = 20;
    particleDamping = 0.15;
    keepTextOn = true;
    textRadius = 30;
    centeringForce = 0.3;
    windStrength = 10;
    windResolution = 300;
    particleSize = 1;
    
    
}

void ofApp::getForces() {
    
    forces.clear();
    
    internal.clear();
    external.clear();
    
    vector<glm::vec3> points;
    for (int i = 0; i < 30; i++) points.push_back(glm::vec3(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 0));
    
    ofxDelaunay tri;
    tri.addPoints(points);
    tri.triangulate();
    
    ofMesh mesh = tri.triangleMesh;
    int n = mesh.getVertices().size();
    
    int adj[n][n];
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) adj[i][j] = 0;
    for (int i = 0; i < mesh.getIndices().size(); i++) {
        if (i % 3 == 0) {
            adj[mesh.getIndices()[i]][mesh.getIndices()[i + 1]] = 1;
            adj[mesh.getIndices()[i]][mesh.getIndices()[i + 2]] = 1;
        } else if (i % 3 == 1) {
            adj[mesh.getIndices()[i]][mesh.getIndices()[i - 1]] = 1;
            adj[mesh.getIndices()[i]][mesh.getIndices()[i + 1]] = 1;
        } else {
            adj[mesh.getIndices()[i]][mesh.getIndices()[i - 1]] = 1;
            adj[mesh.getIndices()[i]][mesh.getIndices()[i - 2]] = 1;
        }
    }
    
    vector<int> cycles[n];
    for (int i = 0; i < n; i++) {
        vector<tuple<float, int>> angleCycle;
        for (int j = 0; j < n; j++) {
            if (adj[i][j] == 1) {
                float angle = atan2(mesh.getVertices()[i].y - mesh.getVertices()[j].y, mesh.getVertices()[i].x - mesh.getVertices()[j].x);
                angleCycle.push_back(make_tuple(angle, j));
            }
        }
        sort(angleCycle.begin(), angleCycle.end());
        
        vector<int> cycle;
        for (int j = 0; j < angleCycle.size(); j++) {
            cycle.push_back(get<1>(angleCycle[j]));
        }
        cycles[i] = cycle;
    }
    
    for (int i = 0; i < n; i++) {
        bool isInternal = true;
        vector<int> thisCycle = cycles[i];
        vector<int> thisCycleSorted = cycles[i];
        sort(thisCycleSorted.begin(), thisCycleSorted.end());
        for (int j = 0; j < thisCycleSorted.size(); j++) {
            vector<int> otherCycle = cycles[thisCycleSorted[j]];
            vector<int> otherCycleSorted = cycles[thisCycleSorted[j]];
            sort(otherCycleSorted.begin(), otherCycleSorted.end());
            vector<int> intersection;
            set_intersection(thisCycleSorted.begin(), thisCycleSorted.end(), otherCycleSorted.begin(), otherCycleSorted.end(), back_inserter(intersection));
            if (intersection.size() < 2) {
                isInternal = false;
                break;
            }
        }
        if (isInternal) {
            internal[i] = thisCycle;
        } else {
            external[i] = 100;
        }
    }
    
    cp.pack(internal, external);
    vector<circle> packing = cp.getPacking();
    
    float xMin = 0, yMin = 0, xMax = 0, yMax = 0;
    for (int i = 0; i < packing.size(); i++) {
        float xPos = packing[i].x;
        float yPos = packing[i].y;
        float r = packing[i].r;
        if (xPos - r < xMin) xMin = xPos - r;
        if (xPos + r > xMax) xMax = xPos + r;
        if (yPos - r < yMin) yMin = yPos - r;
        if (yPos + r > yMax) yMax = yPos + r;
    }
    
    ofRectangle bb(xMin, yMin, xMax - xMin, yMax - yMin);
    
    ofRectangle bbScaled = bb;
    bbScaled.scaleTo(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    
    ofVec2f target(-bb.x - bb.getWidth() / 2, -bb.y - bb.getHeight() / 2);
    float scale = 0.95 * bbScaled.getWidth() / bb.getWidth();
    
    for (int i = 0; i < packing.size(); i++) {
        circle c = packing[i];
        int steps = 2 * PI * c.r / 5;
        for (int j = 0; j < steps; j++) {
            float a1 = ofMap(j, 0, steps, 0, 2 * PI);
            float a2 = ofMap(j + 1, 0, steps, 0, 2 * PI);
            
            float shrinkRadius = 0.75;
            ofVec2f p1(shrinkRadius * c.r * cos(a1) + c.x, shrinkRadius * c.r * sin(a1) + c.y);
            ofVec2f p2(shrinkRadius * c.r * cos(a2) + c.x, shrinkRadius * c.r * sin(a2) + c.y);
            
            p1 += target;
            p2 += target;
            
            p1 *= scale;
            p2 *= scale;
            
            p1 += ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
            p2 += ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
            
            force tmp;
            tmp.x = p1.x;
            tmp.y = p1.y;
            tmp.dx = p2.x - p1.x;
            tmp.dy = p2.y - p1.y;
            forces.push_back(tmp);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (ofGetElapsedTimef() - timeSinceUpdate > 15) {
        if (ofGetElapsedTimef() - timeSinceUpdate > 25) {
            timeSinceUpdate = ofGetElapsedTimef();
            getForces();
            keepTextOn = true;
        } else {
            keepTextOn = false;
        }
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    particleSystem.setTimeStep(timeStep);
    
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, lineOpacity);
    particleSystem.setupForces();
    // apply per-particle forces
    glBegin(GL_LINES);
    for(int i = 0; i < particleSystem.size(); i++) {
        Particle& cur = particleSystem[i];
        // global force on other particles
        particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
//        cur.bounceOffWalls(0, 0, ofGetWidth(), ofGetHeight());
        cur.passThroughWalls(0, 0, ofGetWidth(), ofGetHeight());
        cur.addDampingForce(particleDamping);
    }
    glEnd();
    // single global forces
    if(isMousePressed || keepTextOn) {
//        particleSystem.addRepulsionForce(mouseX, mouseY, 100, 20);
        for (int i = 0; i < forces.size(); i++) {
            particleSystem.addDirectedForce(forces[i].x, forces[i].y, ofVec2f(forces[i].dx, forces[i].dy), 10, textForce);
            particleSystem.addForce(forces[i].x, forces[i].y, 10, textRadius, -textForce);
        }
    } else {
//        particleSystem.addAttractionForce(ofGetWidth() / 2, ofGetHeight() / 2, ofGetWidth(), centerAttraction);
    }
    particleSystem.addLineForce(ofGetWidth() / 2, ofGetHeight() / 2, 0.4 * ofGetHeight(), 2 * ofGetWidth(), -centeringForce);
    
    particleSystem.update(ofGetLastFrameTime(), windStrength, windResolution, ofGetElapsedTimef());
    ofSetColor(255, 255, 255, pointOpacity);
    particleSystem.draw(particleSize);
    ofDisableAlphaBlending();
    
//    if (!saveScreen) {
//        gui.draw();
//
//        ofTranslate(0, ofGetHeight() - 115);
//        ofSetColor(255);
//        ofDrawRectangle(30, 22, 315, 75);
//        ofSetColor(255, 0, 0);
//        ofDrawBitmapString(ofToString(kParticles) + "k particles", 32, 32);
//        ofDrawBitmapString("running at " + ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
//        ofDrawBitmapString("hold down the mouse to reveal the text", 32, 72);
//        ofDrawBitmapString("press 'p' to save a screenshot", 32, 92);
//    } else {
//        ofSaveScreen(ofToString(ofGetMinutes()) + "_" + ofToString(ofGetFrameNum()) + ".png");
//        saveScreen = false;
//    }
    
    cout << ofGetFrameRate() << endl;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'p') ofSaveScreen(ofToString(ofGetMinutes()) + "_" + ofToString(ofGetFrameNum()) + ".png");
    if (key == ' ') getForces();
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
    isMousePressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    isMousePressed = false;
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
