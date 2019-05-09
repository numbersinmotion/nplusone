#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"
#include "ofxDelaunay.h"
#include "ofxCirclepack.h"

struct force {
    float x, y, dx, dy;
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
    
//    float timeStep;
    int lineOpacity, pointOpacity;
    float particleNeighborhood;
//    float particleRepulsion;
//    float centerAttraction;
    
    int kParticles;
    ParticleSystem particleSystem;
    bool isMousePressed, slowMotion;
    
    bool saveScreen = false;
    
    map<int, vector<int>> internal;
    map<int, float> external;
    ofxCirclepack cp;
    
    void getForces();
    vector<force> forces;
    
    float timeSinceUpdate = 0;
    
    float timeStep;
    float particleRepulsion;
    float textForce;
    float particleDamping;
    bool keepTextOn;
    float textRadius;
    float centeringForce;
    float windStrength;
    float windResolution;
    float particleSize;
		
};
