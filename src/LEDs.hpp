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
#include "ofxGPIO.h"
#endif

#endif /* LEDs_hpp */

class LEDs {
public:
    
    ofParameterGroup LEDs;
    // Tailes des strips:
    ofParameter<int>
    numLedsPerStrip{30}, numStrips{8},
    animationBrightness{20}, flashBrightness{31};
    
    ofParameter<ofColor>
    loaderLColor, loaderRColor;
    
    ofParameter<bool> draw;
    ofParameterGroup drawCoords;
    ofParameter<int>
    X{320}, Y{120},
    W{640}, H{480};
    
    ofColor black{0,0,0}, white{255,255,255};
    
    int numLed{0};
    
    vector<ofColor> colors;
    
    ofPixels pixels;
    ofImage img;
    
#ifdef TARGET_RASPBERRY_PI
    LED apa;
#endif
    
    void setup();
    void setup_GUI();
    void question_counters(float index);
    void flash();
    void blackout();
    void update();
    void exit();
    
    
};
