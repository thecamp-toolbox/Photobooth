//
//  Cameras.hpp
//  Photobooth
//
//  Created by Pascal Baltazar on 21/05/2019.
//

#ifndef Cameras_hpp
#define Cameras_hpp

#include "ofMain.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#ifdef TARGET_RASPBERRY_PI
#include "ofxRPiCameraVideoGrabber.h"
#endif

class Cameras {
public:
    ofParameterGroup cameras;
    // Tailes des caméras:
    ofParameter<int>
    camXOffset{320}, camYOffset{120},
    camXsize{640}, camYsize{480};

    ofParameter<float>
    brightnessT{2}, contrastT{2},
    brightnessB{1}, contrastB{1};

    void setup();
    void setup_GUI();
    void update_one();
    void update_all();
    void draw_one(float x, float y, float w, float h);
    void draw_all(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
    void exit();

    ofVideoGrabber USBCam;
    int USBCamNr{0};
#ifdef TARGET_RASPBERRY_PI
    ofxRPiCameraVideoGrabber piCam;
    OMXCameraSettings omxCameraSettings;
#else
    ofVideoGrabber piCam;
    int piCamNr{1};
#endif

    ofShader BCSA_B, BCSA_T;

    ofTexture texPicam;

    bool current{0};
    int nCams{0};

    int maxCamW{1280},
        maxCamH{720};

};

#endif /* Cameras_hpp */
