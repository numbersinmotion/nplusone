#include "ofApp.h"

ofVec2f getVector(ofVec2f p, float t) {
    float epsilon = 0.1;
    float n1, n2, n3, n4;
    p += ofVec2f(1000, 1000);
    p /= 500;
    n1 = ofNoise(p.x, p.y + epsilon);
    n2 = ofNoise(p.x, p.y - epsilon);
    n3 = ofNoise(p.x + epsilon, p.y);
    n4 = ofNoise(p.x - epsilon, p.y);
    ofVec2f v((n1 - n2) / (2 * epsilon), (n3 - n4) / (2 * epsilon));
    return 10 * v;
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFullscreen(true);
    
    ofSetFrameRate(30);

    ofBackground(ofColor(242, 224, 201));
    
    sep = 10;
    dSep = 0.5;
    ess.setup(sep, dSep, ofGetWidth(), ofGetHeight());
    
    vector<ofPolyline> lines = ess.getStreamlines(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), getVector);
    
    meshes = ess.getMeshes();
    
    for (int i = 0; i < lines.size(); i++) {
        pline tmpPline;
        tmpPline.line = lines[i].getResampledBySpacing(2);
        plines.push_back(tmpPline);
    }
    
//    colors.push_back(ofColor(0));
//    colors.push_back(ofColor(255, 0, 0));
//    colors.push_back(ofColor(0, 255, 0));
//    colors.push_back(ofColor(0, 0, 255));
    colors.push_back(ofColor(217, 48, 62));
    colors.push_back(ofColor(17, 110, 191));
    colors.push_back(ofColor(7, 140, 78));
    
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    
    colorIndex = 0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < plines.size(); i++) {
        plines[i].update();
    }
    
    if (ofGetFrameNum() % (150) == 0) {
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        ofColor c = colors[colorIndex % colors.size()];
        for (int i = 0; i < plines.size(); i++) {
            plines[i].addLine(x, y, c);
        }
        colorIndex++;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    for (int i = 0; i < plines.size(); i++) {
//        for (int j = 0; j < plines[i].lines.size(); j++) {
//            ofSetColor(plines[i].colors[j]);
//            plines[i].lines[j].draw();
//        }
//    }

//    for (int n = 0; n < plines.size(); n++) {
//        vector<ofPolyline> lines = plines[n].lines;
//        for (int i = 0; i < lines.size(); i++) {
//
//            ofPolyline line = lines[i];
//            ofColor c = plines[n].colors[i];
//            if (line.size() > 4) {
//                ofMesh mesh, mesh2;
//                mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//                mesh2.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//                for (int j = 1; j < line.size() - 1; j++) {
//
//                    ofVec2f curr = line.getVertices()[j];
//                    ofVec2f prev, next;
//                    if (j > 0) prev = line.getVertices()[j - 1];
//                    if (j < line.size() - 1) next = line.getVertices()[j + 1];
//
//                    float minDist = sep;
//                    vector<particle> neigh = ess.getNeighbors(curr.x, curr.y);
//                    for (int k = 0; k < neigh.size(); k++) {
//                        float dist = ofDist(curr.x, curr.y, neigh[k].pos.x, neigh[k].pos.y);
//                        if (dist < minDist && n != neigh[k].index) minDist = dist;
//                    }
//
//                    float mag = (minDist - dSep * sep) / (sep - dSep * sep) + 0.5;
//
//                    ofVec2f dir;
//                    if (j == 0) dir = next - curr;
//                    if (j == line.size() - 1) dir = curr - prev;
//                    else dir = next - prev;
//                    dir.normalize();
//                    dir.rotate(90);
//                    dir *= 2 * mag;
//
//                    glm::vec3 p1(curr.x + dir.x, curr.y + dir.y, 0);
//                    glm::vec3 p2(curr.x - dir.x, curr.y - dir.y, 0);
//                    glm::vec3 p3(curr.x + 1 * dir.x + 3, curr.y + 1 * dir.y + 3, 0);
//                    glm::vec3 p4(curr.x - 1 * dir.x + 3, curr.y - 1 * dir.y + 3, 0);
//
//                    mesh.addVertex(p1);
//                    mesh.addVertex(p2);
//                    mesh.addColor(c);
//                    mesh.addColor(c);
//                    mesh2.addVertex(p3);
//                    mesh2.addVertex(p4);
//                    mesh2.addColor(ofColor(200));
//                    mesh2.addColor(ofColor(200));
//                }
//                mesh2.draw();
//                mesh.draw();
//            }
//        }
//    }
    
    
//    for (int i = 0; i < meshes.size(); i++) {
//        ofMesh tmpMesh;
//        tmpMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//        for (int j = 0; j < meshes[i].size(); j++) {
//            tmpMesh.addVertex(meshes[i][j]);
//            tmpMesh.addColor(ofColor(0));
//        }
//        tmpMesh.draw();
//    }
    
    for (int i = 0; i < plines.size(); i++) {

        vector<glm::vec3> ref = meshes[i];

        for (int j = 0; j < plines[i].lines.size(); j++) {
            
            ofColor c = plines[i].colors[j];

            if (plines[i].lines[j].size() > 1) {
                
                int endIndex = plines[i].currIndex[j];
                int startIndex = plines[i].currIndex[j] - plines[i].line.size() / 2;
                if (plines[i].reverse[j]) {
                    endIndex = plines[i].currIndex[j] + plines[i].line.size() / 2;
                    startIndex = plines[i].currIndex[j];
                }

                startIndex = MAX(startIndex, 0);
                endIndex = MIN(endIndex, plines[i].line.size());
                
                ofMesh mesh, mesh2;
                mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
                mesh2.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
                for (int k = startIndex; k < endIndex; k++) {
                    
                    float brightness = ofMap(k, plines[i].currIndex[j], plines[i].currIndex[j] + plines[i].line.size() / 2, 255, 0);
                    if (!plines[i].reverse[j]) {
                        brightness = 255 - brightness;
                    }
                    
                    mesh.addVertex(ref[2 * k]);
                    mesh.addVertex(ref[2 * k + 1]);
                    mesh.addColor(ofColor(c, brightness));
                    mesh.addColor(ofColor(c, brightness));
                    mesh2.addVertex(ref[2 * k] + glm::vec3(3, 3, 0));
                    mesh2.addVertex(ref[2 * k + 1] + glm::vec3(3, 3, 0));
                    mesh2.addColor(ofColor(200, brightness));
                    mesh2.addColor(ofColor(200, brightness));

                }

//                mesh2.draw();
                mesh.draw();
            }
        }
    }

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

    for (int i = 0; i < plines.size(); i++) {
        
        ofColor c = colors[colorIndex % colors.size()];
        plines[i].addLine(x, y, c);
        colorIndex++;
    }
    
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
