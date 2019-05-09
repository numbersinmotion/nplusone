#include "Particle.h"

#include "ofGraphics.h"

Particle::Particle(float x, float y, float xv, float yv, float z) :
x(x), y(y), z(z),
xv(xv), yv(yv) {
}

void Particle::updatePosition(float timeStep) {
    // f = ma, m = 1, f = a, v = int(a)
    xv += xf * timeStep;
    yv += yf * timeStep;
    x += xv * timeStep;
    y += yv * timeStep;
}

void Particle::resetForce() {
    xf = 0;
    yf = 0;
}

void Particle::bounceOffWalls(float left, float top, float right, float bottom, float damping) {
    bool collision = false;
    
    if (x > right){
        x = right;
        xv *= -1;
        collision = true;
    } else if (x < left){
        x = left;
        xv *= -1;
        collision = true;
    }
    
    if (y > bottom){
        y = bottom;
        yv *= -1;
        collision = true;
    } else if (y < top){
        y = top;
        yv *= -1;
        collision = true;
    }
    
    if (collision == true){
        xv *= damping;
        yv *= damping;
    }
}

void Particle::passThroughWalls(float left, float top, float right, float bottom) {
    
    if (x > right){
        x = left;
    } else if (x < left){
        x = right;
    }
    
    if (y > bottom){
        y = top;
    } else if (y < top){
        y = bottom;
    }
    
}

void Particle::addDampingForce(float damping) {
    xf -= xv * damping;
    yf -= yv * damping;
}

void Particle::addRandomForce(float size, float resolution, float time) {
//    float angle = PI * ofNoise(x / resolution, y / resolution, time) - PI / 2;
    float angle = 4 * PI * ofNoise(x / resolution, y / resolution, time);
    ofVec2f direction(cos(angle), sin(angle));
    direction *= size;
    xf += direction.x;
    yf += direction.y;
}

void Particle::draw() {
    glVertex2f(x, y);
}
