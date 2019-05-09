#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
timeStep(100) {
}

void ParticleSystem::setup(int width, int height, int k) {
    this->width = width;
    this->height = height;
    this->k = k;
    binSize = 1 << k;
    xBins = (int) ceil((float) width / (float) binSize);
    yBins = (int) ceil((float) height / (float) binSize);
    bins.resize(xBins * yBins);
}

void ParticleSystem::clearParticles() {
    particles.clear();
}

void ParticleSystem::setTimeStep(float timeStep) {
    this->timeStep = timeStep;
}

void ParticleSystem::add(Particle particle) {
    particles.push_back(particle);
}

unsigned ParticleSystem::size() const {
    return particles.size();
}

Particle& ParticleSystem::operator[](unsigned i) {
    return particles[i];
}

vector<Particle*> ParticleSystem::getNeighbors(Particle& particle, float radius) {
    return getNeighbors(particle.x, particle.y, radius);
}

vector<Particle*> ParticleSystem::getNeighbors(float x, float y, float radius) {
    vector<Particle*> region = getRegion(
                                               (int) (x - radius),
                                               (int) (y - radius),
                                               (int) (x + radius),
                                               (int) (y + radius));
    vector<Particle*> neighbors;
    int n = region.size();
    float xd, yd, rsq, maxrsq;
    maxrsq = radius * radius;
    for(int i = 0; i < n; i++) {
        Particle& cur = *region[i];
        xd = cur.x - x;
        yd = cur.y - y;
        rsq = xd * xd + yd * yd;
        if(rsq < maxrsq)
            neighbors.push_back(region[i]);
    }
    return neighbors;
}

vector<Particle*> ParticleSystem::getRegion(unsigned minX, unsigned minY, unsigned maxX, unsigned maxY) {
    vector<Particle*> region;
    back_insert_iterator< vector<Particle*> > back = back_inserter(region);
    unsigned minXBin = minX >> k;
    unsigned maxXBin = maxX >> k;
    unsigned minYBin = minY >> k;
    unsigned maxYBin = maxY >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& cur = bins[y * xBins + x];
            copy(cur.begin(), cur.end(), back);
        }
    }
    return region;
}

void ParticleSystem::setupForces() {
    int n = bins.size();
    for(int i = 0; i < n; i++) {
        bins[i].clear();
    }
    n = particles.size();
    unsigned xBin, yBin, bin;
    for(int i = 0; i < n; i++) {
        Particle& cur = particles[i];
        cur.resetForce();
        xBin = ((unsigned) cur.x) >> k;
        yBin = ((unsigned) cur.y) >> k;
        bin = yBin * xBins + xBin;
        if(xBin < xBins && yBin < yBins)
            bins[bin].push_back(&cur);
    }
}

void ParticleSystem::addRepulsionForce(const Particle& particle, float radius, float scale) {
    addRepulsionForce(particle.x, particle.y, radius, scale);
}

void ParticleSystem::addRepulsionForce(float x, float y, float radius, float scale) {
    addForce(x, y, radius, scale);
}

void ParticleSystem::addAttractionForce(const Particle& particle, float radius, float scale) {
    addAttractionForce(particle.x, particle.y, radius, scale);
}

void ParticleSystem::addAttractionForce(float x, float y, float radius, float scale) {
    addForce(x, y, radius, -scale);
}

void ParticleSystem::addForce(const Particle& particle, float radius, float scale) {
    addForce(particle.x, particle.y, radius, -scale);
}

void ParticleSystem::addForce(float targetX, float targetY, float radius, float scale) {
    float minX = targetX - radius;
    float minY = targetY - radius;
    float maxX = targetX + radius;
    float maxY = targetY + radius;
    if(minX < 0)
        minX = 0;
    if(minY < 0)
        minY = 0;
    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    float xd, yd, length, maxrsq;
#ifdef USE_INVSQRT
    float xhalf;
    int lengthi;
#else
    float effect;
#endif
    maxrsq = radius * radius;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                Particle& curParticle = *(curBin[i]);
                xd = curParticle.x - targetX;
                yd = curParticle.y - targetY;
                length = xd * xd + yd * yd;
                if(length > 0 && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curParticle.x, curParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= radius;
                    length = 1 / length;
                    length = (1 - length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    length *= scale;
                    xd *= length;
                    yd *= length;
                    curParticle.xf += xd;
                    curParticle.yf += yd;
#else
                    length = sqrtf(length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    xd /= length;
                    yd /= length;
                    effect = (1 - (length / radius)) * scale;
                    curParticle.xf += xd * effect;
                    curParticle.yf += yd * effect;
#endif
                }
            }
        }
    }
}

void ParticleSystem::addForce(float targetX, float targetY, float minRadius, float maxRadius, float scale) {
    float minX = targetX - maxRadius;
    float minY = targetY - maxRadius;
    float maxX = targetX + maxRadius;
    float maxY = targetY + maxRadius;
    if(minX < 0)
        minX = 0;
    if(minY < 0)
        minY = 0;
    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    float xd, yd, length, maxrsq, minrsq;
#ifdef USE_INVSQRT
    float xhalf;
    int lengthi;
#else
    float effect;
#endif
    maxrsq = maxRadius * maxRadius;
    minrsq = minRadius * minRadius;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                Particle& curParticle = *(curBin[i]);
                xd = curParticle.x - targetX;
                yd = curParticle.y - targetY;
                length = xd * xd + yd * yd;
                if(length > minrsq && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curParticle.x, curParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= maxRadius;
                    length = 1 / length;
                    length = (1 - length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    length *= scale;
                    xd *= length;
                    yd *= length;
                    curParticle.xf += xd;
                    curParticle.yf += yd;
#else
                    length = sqrtf(length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    xd /= length;
                    yd /= length;
                    effect = (1 - (length / maxRadius)) * scale;
                    curParticle.xf += xd * effect;
                    curParticle.yf += yd * effect;
#endif
                }
            }
        }
    }
}

void ParticleSystem::addDirectedForce(float targetX, float targetY, ofVec2f direction, float radius, float scale) {
    
    direction.normalize();
    
    float minX = targetX - radius;
    float minY = targetY - radius;
    float maxX = targetX + radius;
    float maxY = targetY + radius;
    if(minX < 0)
        minX = 0;
    if(minY < 0)
        minY = 0;
    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    float xd, yd, length, maxrsq;
    
#ifdef USE_INVSQRT
    float xhalf;
    int lengthi;
#else
    float effect;
#endif
    
    maxrsq = radius * radius;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                Particle& curParticle = *(curBin[i]);
                xd = curParticle.x - targetX;
                yd = curParticle.y - targetY;
                length = xd * xd + yd * yd;
                if(length > 0 && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curParticle.x, curParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= radius;
                    length = 1 / length;
                    length = (1 - length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    length *= scale;
                    xd *= length;
                    yd *= length;
                    curParticle.xf += direction.x;
                    curParticle.yf += direction.y;
#else
                    length = sqrtf(length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    direction.x /= length;
                    direction.y /= length;
                    effect = (1 - (length / radius)) * scale;
                    curParticle.xf += direction.x * effect;
                    curParticle.yf += direction.y * effect;
#endif
                }
            }
        }
    }
}

void ParticleSystem::addDirectedForce(float targetX, float targetY, ofVec2f direction, float radius, float scale, float limit) {
    
    direction.normalize();
    
    float minX = targetX - radius;
    float minY = targetY - radius;
    float maxX = targetX + radius;
    float maxY = targetY + radius;
    if(minX < 0)
        minX = 0;
    if(minY < 0)
        minY = 0;
    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    float xd, yd, length, maxrsq;
    
#ifdef USE_INVSQRT
    float xhalf;
    int lengthi;
#else
    float effect;
#endif
    
    maxrsq = radius * radius;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                Particle& curParticle = *(curBin[i]);
                xd = curParticle.x - targetX;
                yd = curParticle.y - targetY;
                length = xd * xd + yd * yd;
                if(length > 0 && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curParticle.x, curParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= radius;
                    length = 1 / length;
                    length = (1 - length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    length *= scale;
                    xd *= length;
                    yd *= length;
                    curParticle.xf += ofClamp(direction.x, -limit, limit);
                    curParticle.yf += ofClamp(direction.y, -limit, limit);
#else
                    length = sqrtf(length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    direction.x /= length;
                    direction.y /= length;
                    effect = (1 - (length / radius)) * scale;
                    curParticle.xf += ofClamp(direction.x * effect, -limit, limit);
                    curParticle.yf += ofClamp(direction.y * effect, -limit, limit);
#endif
                }
            }
        }
    }
}

void ParticleSystem::addLineForce(float targetX, float targetY, float minRadius, float maxRadius, float scale) {
    
    float minX = targetX - maxRadius;
    float minY = targetY - maxRadius;
    float maxX = targetX + maxRadius;
    float maxY = targetY + maxRadius;
    if(minX < 0)
        minX = 0;
    if(minY < 0)
        minY = 0;
    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    maxXBin++;
    maxYBin++;
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    float xd, yd, length, maxrsq, minrsq;
#ifdef USE_INVSQRT
    float xhalf;
    int lengthi;
#else
    float effect;
#endif
    maxrsq = maxRadius * maxRadius;
    minrsq = minRadius * minRadius;
    for(int y = minYBin; y < maxYBin; y++) {
        for(int x = minXBin; x < maxXBin; x++) {
            vector<Particle*>& curBin = bins[y * xBins + x];
            int n = curBin.size();
            for(int i = 0; i < n; i++) {
                Particle& curParticle = *(curBin[i]);
                xd = curParticle.x - targetX;
                yd = curParticle.y - targetY;
                length = 2 * yd * yd;
                if(length > minrsq && length < maxrsq) {
#ifdef DRAW_FORCES
                    glVertex2f(targetX, targetY);
                    glVertex2f(curParticle.x, curParticle.y);
#endif
#ifdef USE_INVSQRT
                    xhalf = 0.5f * length;
                    lengthi = *(int*) &length;
                    lengthi = 0x5f3759df - (lengthi >> 1);
                    length = *(float*) &lengthi;
                    length *= 1.5f - xhalf * length * length;
                    xd *= length;
                    yd *= length;
                    length *= maxRadius;
                    length = 1 / length;
                    length = (1 - length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    length *= scale;
                    xd *= length;
                    yd *= length;
//                    curParticle.xf += xd;
                    curParticle.yf += yd;
#else
                    length = sqrtf(length);
#ifdef USE_SMOOTH_FORCES
                    length = smoothForce(length);
#endif
                    xd /= length;
                    yd /= length;
                    effect = (1 - (length / maxRadius)) * scale;
//                    curParticle.xf += xd * effect;
                    curParticle.yf += yd * effect;
#endif
                }
            }
        }
    }
    
}

void ParticleSystem::update(float lastTimeStep, float randomSize, float randomResolution, float randomTime) {
    int n = particles.size();
    float curTimeStep = lastTimeStep * timeStep;
    for(int i = 0; i < n; i++) {
        particles[i].addRandomForce(randomSize, randomResolution, randomTime);
        particles[i].updatePosition(curTimeStep);
    }
}

void ParticleSystem::draw(float size) {
    int n = particles.size();
    glBegin(GL_POINTS);
    for(int i = 0; i < n; i++) particles[i].draw();
    glEnd();
}

int ParticleSystem::getWidth() const {
    return width;
}

int ParticleSystem::getHeight() const {
    return height;
}
