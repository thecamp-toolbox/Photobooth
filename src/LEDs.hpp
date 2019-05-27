//
//  LEDs.hpp
//  Photobooth
//
//  Created by Pascal Baltazar on 26/05/2019.
//

#ifndef LEDs_hpp
#define LEDs_hpp

#include "ofMain.h"

#ifdef TARGET_RASPBERRY_PI
#include "wiringPi.h"
#include "wiringPiSPI.h"
#endif

#endif /* LEDs_hpp */

class LEDs {
public:
    
    static const int nProfiles{7};
    
    ofParameterGroup LEDs;
    // Tailes des strips:
    ofParameter<int>
    numLedsPerStrip{30}, numStrips{8},
    animationBrightness{20}, flashBrightness{31};
    
    ofParameter<ofColor>
    loaderLColor, loaderRColor;
    
    ofParameter<ofColor>
    profileColor[nProfiles+1];
    
    float noiseThresh{0.3};
    
    ofParameter<bool> draw;
    ofParameterGroup drawCoords;
    ofParameter<int>
    X{320}, Y{120},
    W{640}, H{480};
    
    ofColor black{0,0,0,0}, white{255,255,255, 255};
    
    int numLed{0};
    
    uint8_t brightness;
    
    ofImage img;
    ofImage noiseImg;
    ofPixels pixels;
    
    ofFbo   fbo;
    
    float index{0};
    float profileCounts[nProfiles+1];
    
    void setup();
    void setup_GUI();
    void render(ofPixels& pix);
    void update();
    void setLEDs();
    void exit();
    
    enum Animations {
        NONE,
        INIT,
        QUESTION,
        COMPILE,
        FLASH
    } currentAnimation{};
    
};
