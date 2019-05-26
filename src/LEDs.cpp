//
//  LEDs.cpp
//  Photobooth
//
//  Created by Pascal Baltazar on 26/05/2019.
//

#include "LEDs.hpp"

void LEDs::setup(){
    numLed = numLedsPerStrip+numStrips;
#ifdef TARGET_RASPBERRY_PI
    apa.setupAPA102();
    apa.clearAPA102(numLed+5);
#endif
    pixels.allocate(numLedsPerStrip, numStrips, OF_PIXELS_RGB);
    img.allocate(numLedsPerStrip, numStrips, OF_IMAGE_COLOR);
    for (size_t i = 0; i < numLed; ++i){
        colors.push_back(black);
        pixels.setColor(i, black);
    }
}

void LEDs::setup_GUI(){
    LEDs.setName("LEDs");
    
    LEDs.add(numLedsPerStrip.set ("nb de LEDs par strip", 30, 0, 60));
    LEDs.add(numStrips.set("nb de strips", 8, 0, 24));
    LEDs.add(animationBrightness.set ("lum animation", 20, 0, 31));
    LEDs.add(flashBrightness.set("lum flash", 31, 0, 31));
    
    LEDs.add(loaderLColor.set("couleur loader G", {255, 255, 0}));
    LEDs.add(loaderRColor.set("couleur loader D", {0, 0, 255}));
    
    LEDs.add(draw.set("draw LEDs on screen", 0));
    drawCoords.setName("visualisation LEDs");
    drawCoords.add(X.set("position X", 200, 0, ofGetWidth()));
    drawCoords.add(Y.set("position Y", 1100, 0, ofGetHeight()));
    drawCoords.add(W.set("taille X", 400, 0, ofGetWidth()));
    drawCoords.add(H.set("taille Y", 80, 0, ofGetHeight()));
    
    LEDs.add(drawCoords);
    
}

void LEDs::question_counters(float index){
    for (size_t i = 0; i < numStrips; ++i){
        for (size_t j = 0; j < numLedsPerStrip; ++j){
            if (i%2) {
                if (j>=ofMap(index, 0., 1., 0, numLedsPerStrip-1))
                    colors[i*numLedsPerStrip+j] = black;
                else if (i < numStrips/2) colors[i*numLedsPerStrip+j] = loaderLColor;
                else colors[i*numLedsPerStrip+j] = loaderRColor;
            }
            else {
                if (j<=ofMap(index, 0., 1., 0, numLedsPerStrip-1))
                    colors[i*numLedsPerStrip+j] = black;
                else if (i < numStrips/2) colors[i*numLedsPerStrip+j] = loaderLColor;
                else colors[i*numLedsPerStrip+j] = loaderRColor;
            }
        }
    }
#ifdef TARGET_RASPBERRY_PI
    apa.setAPA102(numLed,colors,animationBrightness);
#endif
}

void LEDs::flash(){
    for (size_t i = 0; i < numStrips; ++i){
        for (size_t j = 0; j < numLedsPerStrip; ++j){
            colors[i*numLedsPerStrip+j] = white;
        }
    }
#ifdef TARGET_RASPBERRY_PI
    apa.setAPA102(numLed,colors,flashBrightness);
#endif
}

void LEDs::blackout(){
    for (size_t i = 0; i < numStrips; ++i){
        for (size_t j = 0; j < numLedsPerStrip; ++j){
            colors[i*numLedsPerStrip+j] = black;
        }
    }
#ifdef TARGET_RASPBERRY_PI
    apa.setAPA102(numLed,colors,flashBrightness);
#endif
}

void LEDs::update(){
    for (size_t i = 0; i < numStrips; ++i){
        for (size_t j = 0; j < numLedsPerStrip; ++j){
            pixels.setColor(j, i,colors[i*numLedsPerStrip+j]);
        }
    }
    img.setFromPixels(pixels);
}

void LEDs::exit(){
#ifdef TARGET_RASPBERRY_PI
    apa.clearAPA102(numLed+5);
#endif
}


