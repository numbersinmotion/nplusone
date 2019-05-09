#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    
    lines.clear();
    internal.clear();
    external.clear();
    
    vector<glm::vec3> points;
    for (int i = 0; i < ofRandom(20, 500); i++) points.push_back(glm::vec3(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 0));
    
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
    float scale = bbScaled.getWidth() / bb.getWidth();
    
    bool switchRotation = false;
    if (ofRandom(1) < 0.5) switchRotation = true;
    
    for (int i = 0; i < packing.size(); i++) {
        
        circle c = packing[i];
        vector<int> cycle = cycles[c.key];
        for (int j = 0; j < cycle.size(); j++) {
            circle otherCircle = packing[0];
            for (int k = 0; k < packing.size(); k++) {
                if (packing[k].key == cycle[j]) otherCircle = packing[k];
            }
            
            if (c.key > otherCircle.key) {
                ofVec2f centerDiff(otherCircle.x - c.x, otherCircle.y - c.y);
                
                ofPolyline line;
                for (int k = 0; k < 100; k++) {
                    float a = atan2(centerDiff.y, centerDiff.x);
                    float b = c.r / 2 / PI;
                    float theta = ofMap(k, 0, 99, 0, 2 * PI);
                    float r = b * theta;
                    float x = r * cos(theta + a) + c.x;
                    float y = r * sin(theta + a) + c.y;
                    if (switchRotation) {
                        theta = -ofMap(k, 0, 99, 0, 2 * PI);
                        r = b * theta;
                        x = r * cos(theta + a + PI) + c.x;
                        y = r * sin(theta + a + PI) + c.y;
                    }
                    line.addVertex(glm::vec3(scale * (x + target.x), scale * (y + target.y), 0));
                }
                for (int k = 0; k < 100; k++) {
                    float a = atan2(centerDiff.y, centerDiff.x);
                    float b = otherCircle.r / 2 / PI;
                    float theta = ofMap(k, 0, 99, 2 * PI, 0);
                    float r = b * theta;
                    float x = r * cos(theta + a + PI) + otherCircle.x;
                    float y = r * sin(theta + a + PI) + otherCircle.y;
                    if (switchRotation) {
                        theta = -ofMap(k, 0, 99, 2 * PI, 0);
                        r = b * theta;
                        x = r * cos(theta + a) + otherCircle.x;
                        y = r * sin(theta + a) + otherCircle.y;
                    }
                    line.addVertex(glm::vec3(scale * (x + target.x), scale * (y + target.y), 0));
                }
                line = line.getResampledBySpacing(1);
                lines.push_back(line);
            }
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    
    float lineLength = ofMap(mouseX, 0, ofGetWidth(), 1, 10);
    float lineSpeed = ofMap(mouseY, 0, ofGetHeight(), 30, 200);
    
    for (int i = 0; i < lines.size(); i++) {
        int start = (int)(lineSpeed * ofGetElapsedTimef());
        int end = (int)(start + lines[i].size() / lineLength);
        
        ofPolyline tmpLine;
        for (int j = start; j < end; j++) {
            int index = j % lines[i].size();
            tmpLine.addVertex(lines[i].getVertices()[index].x, lines[i].getVertices()[index].y);
            if (index == lines[i].size() - 1) {
                tmpLine.draw();
                tmpLine.clear();
            }
        }
        tmpLine.draw();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
