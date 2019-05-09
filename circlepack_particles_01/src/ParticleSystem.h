#pragma once

#include "Particle.h"

//#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES

class ParticleSystem {
protected:
    float timeStep;
    vector<Particle> particles;
    vector< vector<Particle*> > bins;
    int width, height, k, xBins, yBins, binSize;
    
public:
    ParticleSystem();
    
    void setup(int width, int height, int k);
    void clearParticles();
    void setTimeStep(float timeStep);
    
    void add(Particle particle);
    vector<Particle*> getNeighbors(Particle& particle, float radius);
    vector<Particle*> getNeighbors(float x, float y, float radius);
    vector<Particle*> getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY);
    unsigned size() const;
    Particle& operator[](unsigned i);
    
    void setupForces();
    void addRepulsionForce(const Particle& particle, float radius, float scale);
    void addRepulsionForce(float x, float y, float radius, float scale);
    void addAttractionForce(const Particle& particle, float radius, float scale);
    void addAttractionForce(float x, float y, float radius, float scale);
    void addForce(const Particle& particle, float radius, float scale);
    void addForce(float x, float y, float radius, float scale);
    void addForce(float x, float y, float minRadius, float maxRadius, float scale);
    void addDirectedForce(float targetX, float targetY, ofVec2f direction, float radius, float scale);
    void addDirectedForce(float targetX, float targetY, ofVec2f direction, float radius, float scale, float limit);
    void addLineForce(float targetX, float targetY, float minRadius, float maxRadius, float scale);
    void update(float lastTimeStep, float randomSize, float randomResolution, float randomTime);
    
    void draw(float size);
    
    int getWidth() const;
    int getHeight() const;
};

inline float InvSqrt(float x){
    float xhalf = 0.5f * x;
    int i = *(int*)&x; // store floating-point bits in integer
    i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
    x = *(float*)&i; // convert new bits into float
    x = x*(1.5f - xhalf*x*x); // One round of Newton's method
    return x;
}

inline float smoothForce(float x) {
    const static float sharpness = 1;
    return 1. / (1. + expf((x - .5) * sharpness * -12));
}
