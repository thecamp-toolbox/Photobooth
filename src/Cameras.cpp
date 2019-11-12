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

#ifdef TARGET_RASPBERRY_PI
    BCSA_B.load("bcsa-Pi");
    BCSA_T.load("bcsa-Pi");
#else
    BCSA_B.load("bcsa-Mac");
    BCSA_T.load("bcsa-Mac");
 #endif
    vector<ofVideoDevice> devices = USBCam.listDevices();
    ofLog() << "Cameras: " ;
    for (auto& c : devices) {
        ofLog() << c.deviceName << " / ID: " << c.id;
        nCams++;
        #ifdef TARGET_RASPBERRY_PI
        #else
        if (c.deviceName.find("FaceTime") == 0) piCamNr = c.id;
        else USBCamNr = c.id;
        #endif
    }
    nCams = devices.size();
    ofLog() << "nombre de caméras: " << nCams;

    ofLog() << " Setup Cam 1 (USB) with Device#" << USBCamNr;
    USBCam.setDeviceID(USBCamNr);
    USBCam.setDesiredFrameRate(30);
    USBCam.setPixelFormat(OF_PIXELS_GRAY);
    USBCam.initGrabber(640, 480);
    ofLog() << "size 1: " << USBCam.getWidth() << " / " << USBCam.getHeight();
#ifdef TARGET_RASPBERRY_PI
    omxCameraSettings.width = 1280; //default 1280
    omxCameraSettings.height = 720; //default 720
    omxCameraSettings.enableTexture = true; //default true
    omxCameraSettings.doRecording = false;   //default false

    piCam.setup(omxCameraSettings);
#else
    ofLog() << " Setup Cam 2 (Facetime) with Device#" << piCamNr;
    piCam.setDeviceID(piCamNr);
    piCam.setDesiredFrameRate(30);
    piCam.setPixelFormat(OF_PIXELS_GRAY);
    piCam.initGrabber(1280, 720);
    ofLog() << "size 2: " << piCam.getWidth() << " / " << piCam.getHeight();
#endif

}

void Cameras::update_one(){
    if (current) USBCam.update();
#ifdef TARGET_RASPBERRY_PI
    texPicam = piCam.getTextureReference();
#else
    else piCam.update();
    texPicam = piCam.getTexture();
#endif

}

void Cameras::update_all(){
    USBCam.update();
#ifdef TARGET_RASPBERRY_PI
    texPicam = piCam.getTextureReference();
#else
    piCam.update();
    texPicam = piCam.getTexture();
#endif

}


void Cameras::draw_one(float x, float y, float w, float h){
    if (current) {
        if (brightnessB!=1&&contrastB!=1){
            ofTexture& videoTexture1 = USBCam.getTexture();
            BCSA_B.begin();
            ofClear(0,0,0);
            BCSA_B.setUniform1f("brightness", brightnessT);
            BCSA_B.setUniform1f("contrast", contrastT);
            BCSA_B.setUniformTexture("image", videoTexture1,1);
            videoTexture1.draw(x+w, y, -w, h);
            BCSA_B.end();
        }
        else USBCam.draw(x+w, y, -w, h);
    }
    else {
        BCSA_T.begin();
        ofClear(0,0,0);
        BCSA_T.setUniform1f("brightness", brightnessT);
        BCSA_T.setUniform1f("contrast", contrastT);
        BCSA_T.setUniform1f("saturation", 0);
        BCSA_T.setUniform1f("alpha", 1.);
        BCSA_T.setUniformTexture("image", texPicam,texPicam.getTextureData().textureID);

        glPushMatrix();
        glTranslatef(200,200,0);
        glRotatef(180, 0, 0, 1); //only rotate around the z axis

        texPicam.drawSubsection(x, y+h, w, -h, camXOffset, camYOffset, camXsize, camYsize);

        ofPopMatrix();

        BCSA_T.end();
    }
}

void Cameras::draw_all(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2){
    BCSA_T.begin();
    ofClear(0,0,0);
    BCSA_T.setUniform3f("avgluma",0.62,0.62,0.62);
    BCSA_T.setUniform1f("brightness", brightnessT);
    BCSA_T.setUniform1f("contrast", contrastT);
    BCSA_T.setUniform1f("saturation", 0);
    BCSA_T.setUniform1f("alpha", 1.);
    BCSA_T.setUniformTexture("image", texPicam,texPicam.getTextureData().textureID);
    texPicam.drawSubsection(x2, y2+w2, w2, -h2, camXOffset, camYOffset, camXsize, camYsize);
    BCSA_T.end();

    if (brightnessB!=1&&contrastB!=1){
        ofTexture& videoTexture2 = USBCam.getTexture();
        BCSA_B.begin();
        ofClear(0,0,0);
        BCSA_B.setUniform3f("avgluma",0.62,0.62,0.62);
        BCSA_B.setUniform1f("brightness", brightnessT);
        BCSA_B.setUniform1f("contrast", contrastT);
        BCSA_B.setUniform1f("saturation", 0);
        BCSA_B.setUniform1f("alpha", 1.);
        BCSA_B.setUniformTexture("image", videoTexture2,1);
        videoTexture2.draw(x1+w1, y1, -w1, h1);
        BCSA_B.end();
    }
    else USBCam.draw(x1+w1, y1, -w1, h1);
}

void Cameras::setup_GUI(){
    cameras.setName("Cameras");

    cameras.add(camXOffset.set ("crop offset X", 320, 0, 360));
    cameras.add(camYOffset.set("crop offset Y", 120, 0, 640));
    cameras.add(camXsize.set ("crop largeur", 640, 0, 360));
    cameras.add(camYsize.set("crop hauteur", 480, 0, 640));
    cameras.add(brightnessT.set("luminosité cam Ht", 2, 0, 4));
    cameras.add(contrastT.set("contraste cam Ht", 2, 0, 4));
    cameras.add(brightnessB.set("luminosité cam Bas", 1, 0, 4));
    cameras.add(contrastB.set("contraste cam Bas", 1, 0, 4));

}

void Cameras::exit(){

}
