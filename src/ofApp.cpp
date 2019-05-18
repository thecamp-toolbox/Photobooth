#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //___________________________
    // Camera setup
    
    // uncomment to get a list of connected cams:
    /*
    vector<ofVideoDevice> devices = cams[0].listDevices();
    ofLog() << "Cameras: " ;
    for (auto& c : devices) ofLog() << c.deviceName << " / ID: " << c.id;
     */
    
    cams[0].setDeviceID(1);
    cams[0].setDesiredFrameRate(30);
    cams[0].setPixelFormat(OF_PIXELS_NATIVE);
    cams[0].setup(camWidth, camHeight);
    
    cams[1].setDeviceID(0);
    cams[1].setDesiredFrameRate(30);
    cams[1].setPixelFormat(OF_PIXELS_NATIVE);
    cams[1].setup(camWidth, camHeight);
    
    ofLog() << "size 1: " << cams[0].getWidth() << " / " << cams[0].getHeight();
    ofLog() << "size 2: " << cams[1].getWidth() << " / " << cams[1].getHeight();
    
    //___________________________
    // Loading Background Images:
    for (size_t i = 0; i < ST_NR; ++i){
        switch(i){
            case QUESTION:
                for (size_t j = 0; j < nQuestions; ++j){
                    questions[j].load("/data/BG/"+backgroundFiles[QUESTION]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[QUESTION]+to_string(j+1)+".png");
                }
                break;
                
            case PROFILE:
                for (size_t j = 0; j < PR_NR; ++j){
                    profiles[j].load("/data/BG/"+backgroundFiles[PROFILE]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[PROFILE]+to_string(j+1)+".png");
                }
                break;
                
            case FRAME:
                for (size_t j = 0; j < PR_NR; ++j){
                    profiles[j].load("/data/BG/"+backgroundFiles[FRAME]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[FRAME]+to_string(j+1)+".png");
                }
                break;
            
            case COUNTDOWN:
                for (size_t j = 0; j < nCountdown; ++j){
                    profiles[j].load("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
                }
                break;
            
            default:
                backgrounds[i].load("/data/BG/"+backgroundFiles[i]);
                ofLog() << ("/data/BG/"+backgroundFiles[i]);
                break;
            
        }
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    switch (currentState) {
        case INIT: {
            
            break;
        }
        case STANDBY: {
            
            break;
        }
        case WELCOME: {
            
            break;
        }
        case EXPLAIN: {
            
            break;
        }
        case QUESTION: {
            
            break;
        }
        case COMPILING: {
            
            break;
        }
        case PROFILE: {
            
            break;
        }
        case CAM_CHOICE: {
            cams[currentCam].update();
            cams[!currentCam].update();
            break;
        }
        case FRAME: {
            cams[currentCam].update();
            break;
        }
        case COUNTDOWN: {
            cams[currentCam].update();
            break;
        }
        case FLASH: {
            
            break;
        }
        case RESULT: {
            
            break;
        }
        case PRINTING: {
            
            break;
        }
        case BYE: {
            
            break;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    switch (currentState) {
        case INIT: {
            
            break;
        }
        case STANDBY: {
            
            break;
        }
        case WELCOME: {
            
            break;
        }
        case EXPLAIN: {
            
            break;
        }
        case QUESTION: {
            
            break;
        }
        case COMPILING: {
            
            break;
        }
        case PROFILE: {
            
            break;
        }
        case CAM_CHOICE: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            cams[!currentCam].draw(posSecCamX, posSecCamY, sizeSecCamX, sizeSecCamY);
            break;
        }
        case FRAME: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            break;
        }
        case COUNTDOWN: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            break;
        }
        case FLASH: {
            
            break;
        }
        case RESULT: {
            
            break;
        }
        case PRINTING: {
            
            break;
        }
        case BYE: {
            
            break;
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    currentCam = !currentCam;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}


