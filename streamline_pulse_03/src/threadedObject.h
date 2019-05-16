#pragma once

#include "ofMain.h"
#include "ofxEvenlySpacedStreamlines.h"
#include <atomic>

class ThreadedObject : public ofThread {

    public:
    
//        ~ThreadedObject() {
//            stop();
//            waitForThread(false);
//        }
    
        void setup() {
            ess.setup(10 * floor(ofRandom(1, 4)), 0.5, ofGetWidth(), ofGetHeight());
            test = 0;
        }
    
//        void start() {
//            startThread();
//        }
    
//        void stop() {
//            std::unique_lock<std::mutex> lck(mutex);
//            stopThread();
//            condition.notify_all();
//        }
    
//        void update() {
//            std::unique_lock<std::mutex> lock(mutex);
//            test++;
//            condition.notify_all();
//        }
    
//        vector<vector<glm::vec3>> getMeshes() {
//            return meshes;
//        }
    
//        int getTest() {
//            return test;
//        }
    
//    protected:
    
        void threadedFunction();
    
//        std::condition_variable condition;
        ofxEvenlySpacedStreamlines ess;
        vector<ofPolyline> lines;
        vector<vector<glm::vec3>> currMeshes;
        int test;
    
};
