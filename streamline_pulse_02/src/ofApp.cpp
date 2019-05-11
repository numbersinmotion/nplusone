#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFullscreen(true);
    
    ofSetFrameRate(30);

    ofBackground(ofColor(242, 224, 201));
    
    colors.push_back(ofColor(217, 48, 62));
    colors.push_back(ofColor(17, 110, 191));
    colors.push_back(ofColor(7, 140, 78));
    
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    
    colorIndex = 0;
    
    threadedObject.setup();
    threadedObject.startThread();
    threadLoading = true;
    clearing = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (threadedObject.isThreadRunning()) {
        cout << "Thread is running... " << ofGetFrameNum() << endl;
    }
    
    if (threadLoading == true && !threadedObject.isThreadRunning()) {
        threadLoading = false;
        clearing = true;
    }
    
    for (int i = 0; i < plines.size(); i++) {
        plines[i].update();
    }
    
    if (!clearing) {
        if (ofGetFrameNum() % (150) == 0) {
            float x = ofRandom(ofGetWidth());
            float y = ofRandom(ofGetHeight());
            ofColor c = colors[colorIndex % colors.size()];
            for (int i = 0; i < plines.size(); i++) {
                plines[i].addLine(x, y, c);
            }
            colorIndex++;
        }
    } else {
        clearing = false;
        for (int i = 0; i < plines.size(); i++) {
            if (plines[i].lines.size() > 0) {
                clearing = true;
                break;
            }
        }
        if (!clearing) {
            plines.clear();
            for (int i = 0; i < threadedObject.lines.size(); i++) {
                pline tmpPline;
                tmpPline.line = threadedObject.lines[i].getResampledBySpacing(2);
                plines.push_back(tmpPline);
            }
            meshes = threadedObject.currMeshes;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
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
                
                mesh.draw();
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    threadedObject.setup();
    threadedObject.startThread();
    threadLoading = true;
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
