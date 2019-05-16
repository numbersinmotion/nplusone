#pragma once

#include "ofMain.h"
#include "ofxEvenlySpacedStreamlines.h"
#include "threadedObject.h"

class pline {
    
public:
    
    ofPolyline line;
    vector<ofPolyline> lines;
    vector<ofColor> colors;
    vector<int> currIndex;
    vector<bool> reverse;
    vector<bool> drawing;
    
    void addLine(float x, float y, ofColor c) {
        
        ofVec2f first = line.getVertices()[0];
        ofVec2f last = line.getVertices()[line.size() - 1];
        
        ofPolyline tmpLine;
        
        float firstDist = ofDist(x, y, first.x, first.y);
        float lastDist = ofDist(x, y, last.x, last.y);
        
        if (firstDist > lastDist) {
            reverse.push_back(true);
            currIndex.push_back(line.size() - 1 + (int)ofMap(lastDist, 0, 100, 0, 20));
        } else {
            reverse.push_back(false);
            currIndex.push_back(-(int)ofMap(firstDist, 0, 100, 0, 20));
        }
        
        colors.push_back(c);
        drawing.push_back(false);
        lines.push_back(tmpLine);
        
    }
    
    void update() {
        
        for (int i = lines.size() - 1; i >= 0; i--) {
            
            if (reverse[i]) {
                currIndex[i]--;
            } else {
                currIndex[i]++;
            }
            
            if (!drawing[i] && ((!reverse[i] && currIndex[i] >= 0) || (reverse[i] && currIndex[i] <= line.size() - 1))) {
                drawing[i] = true;
            }
                
            if (drawing[i]) {
            
                if (currIndex[i] < 0 || currIndex[i] > line.size() - 1) {
                    lines[i].getVertices().erase(lines[i].getVertices().begin());
                } else {
                    lines[i].addVertex(line[currIndex[i]].x, line[currIndex[i]].y);
                }
                
                if (lines[i].size() > line.size() / 2) {
                    lines[i].getVertices().erase(lines[i].getVertices().begin());
                }
                
                if (lines[i].size() == 0) {
                    lines.erase(lines.begin() + i);
                    currIndex.erase(currIndex.begin() + i);
                    reverse.erase(reverse.begin() + i);
                    drawing.erase(drawing.begin() + i);
                    colors.erase(colors.begin() + i);
                }
                
            }
            
        }
        
    }
    
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<pline> plines;
    int plinesStartCount;
    vector<vector<glm::vec3>> meshes;
    
    vector<vector<ofColor>> colorSchemes;
    vector<ofColor> colors;
    
    ofVec2f offset;
    
    int colorSchemeIndex;
    int colorIndex;
    
    ThreadedObject threadedObject;
    bool threadLoading;
    bool threadReady;
    bool clearing;
    
    ofFbo fbo;
    
};
