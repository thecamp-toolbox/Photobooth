//
//  Cameras.cpp
//  Photobooth
//
//  Created by Pascal Baltazar on 21/05/2019.
//

#include "Cameras.hpp"

void Cameras::setup(){
    //___________________________
    // Camera setup
    
    vector<ofVideoDevice> devices = USBcam.listDevices();
    ofLog() << "Cameras: " ;
    for (auto& c : devices) {
        ofLog() << c.deviceName << " / ID: " << c.id;
        nCams++;
    }
    nCams = devices.size();
    ofLog() << "nombre de caméras: " << nCams;
    
    ofLog() << " Setup Cam 1 with Device#" << 0;
    USBcam.setDeviceID(1);
    USBcam.setDesiredFrameRate(30);
    //cams[0].setPixelFormat(OF_PIXELS_NATIVE);
    USBcam.initGrabber(640, 480);
    ofLog() << "size 1: " << USBcam.getWidth() << " / " << USBcam.getHeight();
#ifdef TARGET_RASPBERRY_PI
    omxCameraSettings.width = 1280; //default 1280
    omxCameraSettings.height = 720; //default 720
    omxCameraSettings.enableTexture = true; //default true
    omxCameraSettings.doRecording = false;   //default false
    
    piCam.setup(omxCameraSettings);
#else
    piCam.setDeviceID(0);
    piCam.setDesiredFrameRate(30);
    //cams[0].setPixelFormat(OF_PIXELS_NATIVE);
    piCam.initGrabber(1280, 720);
    ofLog() << "size 2: " << piCam.getWidth() << " / " << piCam.getHeight();
#endif
    
}

void Cameras::update_one(){
    if (current) USBcam.update();
#ifdef TARGET_RASPBERRY_PI
    texPicam piCam.getTextureReference();
#else
    else piCam.update();
    texPicam = piCam.getTexture();
#endif
    
}

void Cameras::update_all(){
    USBcam.update();
#ifdef TARGET_RASPBERRY_PI
    texPicam piCam.getTextureReference();
#else
    piCam.update();
    texPicam = piCam.getTexture();
#endif

}

void Cameras::draw_one(float x, float y, float w, float h){
    if (current) USBcam.draw(x+w, y, -w, h);
    else texPicam.drawSubsection(x+w, y, -w, h, camXOffset, camYOffset, camXsize, camYsize);
}

void Cameras::draw_all(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2){
    USBcam.draw(x1+w1, y1, -w1, h1);
    texPicam.drawSubsection(x2+w2, y2, -w2, h2, camXOffset, camYOffset, camXsize, camYsize);
}

void Cameras::setup_GUI(){
    cameras.setName("Cameras");
    
    cameras.add(camXOffset.set ("crop offset X", 320, 0, maxCamW));
    cameras.add(camYOffset.set("crop offset Y", 120, 0, maxCamH));
    cameras.add(camXsize.set ("crop largeur", 640, 0, maxCamW));
    cameras.add(camYsize.set("crop hauteur", 480, 0, maxCamH));

}

void Cameras::exit(){
    
}
