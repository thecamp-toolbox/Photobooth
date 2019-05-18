#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //cams = new ofVideoGrabber[2];
    
    // Camera setup
    // uncomment to get a list of connected cams:
    vector<ofVideoDevice> devices = cams[0].listDevices();
    ofLog() << "Cameras: " ;
    for (auto& c : devices) ofLog() << c.deviceName << " / ID: " << c.id;
    
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

}

//--------------------------------------------------------------
void ofApp::update(){
    
    cams[0].update();
    cams[1].update();

}

//--------------------------------------------------------------
void ofApp::draw(){

    cams[0].draw(300, 100, 620, 680);
    cams[1].draw(930, 100, 620, 680);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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


