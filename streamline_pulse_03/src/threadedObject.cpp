#include "threadedObject.h"

ofVec2f getVector(ofVec2f p, float t) {
    float epsilon = 0.1;
    float n1, n2, n3, n4;
    p += ofVec2f(1000, 1000);
    p /= 500;
    n1 = ofNoise(p.x, p.y + epsilon, t);
    n2 = ofNoise(p.x, p.y - epsilon, t);
    n3 = ofNoise(p.x + epsilon, p.y, t);
    n4 = ofNoise(p.x - epsilon, p.y, t);
    ofVec2f v((n1 - n2) / (2 * epsilon), (n3 - n4) / (2 * epsilon));
    return 10 * v;
}

void ThreadedObject::threadedFunction() {
//    std::unique_lock<std::mutex> lock(mutex);
//    test++;
    lines = ess.getStreamlines(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), getVector);
    currMeshes = ess.getMeshes();
//    condition.wait(lock);
}
