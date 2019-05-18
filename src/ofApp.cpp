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
    for (size_t i = 0; i < States::END; ++i){
        switch(i){
            case QUESTION:{
                for (size_t j = 0; j < nQuestions; ++j){
                    
                }
            }
            case PROFILE: {
                
            }
            case FRAME: {
                
            }
            default: {
                
            }
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
        case END: {
            
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
        case END: {
            
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


