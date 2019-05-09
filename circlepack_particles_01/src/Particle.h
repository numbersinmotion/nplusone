
#pragma once

#include "ofMain.h"

class Particle {
public:
    float x, y, z;
    float xv, yv;
    float xf, yf;
    Particle(float x = 0, float y = 0, float xv = 0, float yv = 0, float z = 0);
    void updatePosition(float timeStep);
    void resetForce();
    void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3);
    void passThroughWalls(float left, float top, float right, float bottom);
    void addDampingForce(float damping = .01);
    void addRandomForce(float size = 0.01, float resolution = 200, float time = 0);
    void draw();
};
