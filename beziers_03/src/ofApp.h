#pragma once

#include "ofMain.h"
#include "ofxShivaVGRenderer.h"

struct bezierPoint {
    ofPoint p, a;
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
		
    ofPtr<ofxShivaVGRenderer> _shivaVGRenderer;
    
    ofPolyline getBezier(bezierPoint start, bezierPoint end);
    void getNext();
    int numPoints;
    bezierPoint curr;
    bezierPoint next;
    ofPolyline line;
    ofPolyline nextLine;
    
    ofVec2f cameraPosition;
    
    bool debugMode;
    
};