#pragma once

#include "ofMain.h"

struct bezierPoint {
    ofPoint p, a;
};

class BezierWalker {

    public:
    
        void setup(ofColor c, float w, float s, float min, float max);
        void update();
        void draw();
        void drawDebug();
    
        ofPolyline getBezier(bezierPoint start, bezierPoint end);
        void getNext();
        int numPoints;
        bezierPoint curr;
        bezierPoint next;
        ofPolyline line;
        ofPolyline nextLine;
    
        ofColor color;
        float width;
        float speed;
        float minRadius;
        float maxRadius;
    
};
