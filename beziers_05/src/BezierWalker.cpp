#include "BezierWalker.h"

void BezierWalker::setup(ofColor c, float w, float s, float min, float max) {
    
    color = c;
    width = w;
    speed = s;
    minRadius = min;
    maxRadius = max;
    
    numPoints = 2000;
    
    float angle, radius;
    
    curr.p = ofPoint(ofRandom(ofGetWidth() * 0.2, ofGetWidth() * 0.8), ofRandom(ofGetHeight() * 0.2, ofGetHeight() * 0.8));
    angle = ofRandom(2 * PI);
    radius = ofRandom(200, 300);
    curr.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    next.p = ofPoint(ofRandom(ofGetWidth() * 0.2, ofGetWidth() * 0.8), ofRandom(ofGetHeight() * 0.2, ofGetHeight() * 0.8));
    angle = ofRandom(2 * PI);
    radius = ofRandom(200, 300);
    next.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    line = getBezier(curr, next);
    getNext();
    
    while (line.getVertices().size() < numPoints) {
        line.addVertices(nextLine.getVertices());
        getNext();
    }
    
}

void BezierWalker::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    line.addVertex(nextLine.getVertices()[0]);
    
    nextLine.getVertices().erase(nextLine.getVertices().begin());
    
    if (nextLine.size() == 0) getNext();
    
    line.getVertices().erase(line.getVertices().begin());
    
}

void BezierWalker::draw(){
    
    ofSetLineWidth(width);
    ofSetColor(color);
    line.draw();
    
}

void BezierWalker::drawDebug(){
    
    ofSetLineWidth(1.0f);
    ofSetColor(255, 0, 0);
    ofDrawCircle(curr.p, 5);
    ofSetColor(255, 0, 50);
    ofDrawCircle(curr.p + curr.a, 5);
    ofSetColor(255);
    ofDrawLine(curr.p, curr.p + curr.a);
    ofSetColor(0, 255, 0);
    ofDrawCircle(next.p, 5);
    ofSetColor(0, 255, 50);
    ofDrawCircle(next.p - next.a, 5);
    ofSetColor(255);
    ofDrawLine(next.p, next.p - next.a);
    ofSetColor(255);
    ofDrawLine(curr.p + curr.a, next.p - next.a);
    
}

ofPolyline BezierWalker::getBezier(bezierPoint start, bezierPoint end) {
    
    ofPoint a = start.p;
    ofPoint b = start.p + start.a;
    ofPoint c = end.p - end.a;
    ofPoint d = end.p;
    
    ofPolyline out;
    for (float i = 0; i <= 1; i += 0.01) {
        ofPoint p = ofBezierPoint(a, b, c, d, i);
        out.addVertex(p);
    }
    
    return out.getResampledBySpacing(speed);
    
}

void BezierWalker::getNext() {
    
    curr = next;
    
    float angle, radius;
    
    angle = ofRandom(2 * PI);
    radius = ofRandom(minRadius, maxRadius);
    next.p = curr.p + ofVec2f(radius * cos(angle), radius * sin(angle));
    next.p = ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    angle = atan2(curr.a.y, curr.a.x) + PI / 2;
    radius = ofRandom(minRadius, maxRadius);
    next.a = ofPoint(radius * cos(angle), radius * sin(angle));
    
    nextLine = getBezier(curr, next);
    
}
