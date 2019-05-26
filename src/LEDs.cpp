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
    wiringPiSetup();
    if(wiringPiSPISetup(0,7812500)<0){
        ofLog() << "Failed to setup SPI!";
    }
    else{
        ofLog()<<"Open setup SPI!";
        wiringPiSetupSys() ;
    }
#endif
    noise.allocate(numStrips, numLedsPerStrip, 3);
    //img.allocate(numStrips,numLedsPerStrip, OF_IMAGE_COLOR_ALPHA);
    fbo.allocate(numStrips,numLedsPerStrip,GL_RGB);
    fbo.begin();
        ofClear(black);
        img.grabScreen(0, 0, numStrips,numLedsPerStrip);
    fbo.end();
    setLEDs();
    //pixels = img.getPixels();
    //img.update();
    //pixels.setColor(black);
    //blackout();

}

void LEDs::update(){
    
    fbo.begin();
    ofClear(black);
    
    switch (currentAnimation) {
        case NONE:
            ofSetColor(black);
            ofDrawRectangle(0, 0, numStrips,numLedsPerStrip);
            brightness = 0;
            break;
            
        case INIT:
            ofSetColor(loaderLColor);
            ofDrawRectangle(0, 0, numStrips/2,numLedsPerStrip);
            ofSetColor(loaderRColor);
            ofDrawRectangle(numStrips/2, 0, numStrips,numLedsPerStrip);
            brightness = animationBrightness;
            break;
        
        case COMPILE:
            ofSetColor(black);
            // make some noise
            
            /*
            for (size_t i = 0; i < nProfiles+1; ++i){
                for (size_t j = 0; j < numLedsPerStrip*(1-index*(1-profileCounts[i]))-1; ++j){
                    if (ofRandom(1) > noiseThresh) noise.setColor(i, j, (white.lerp(profileColor[i], index)));
                    else noise.setColor(i, j, black);
                }
            }
            noiseImg.setFromPixels(noise);
            noiseImg.draw(0, 0, nProfiles+1 ,numLedsPerStrip);
            */
            
            for (size_t i = 0; i < nProfiles+1; ++i){
                ofSetColor((white.lerp(profileColor[i], index)));
                for (size_t j = 0; j < numLedsPerStrip*(1-index*(1-profileCounts[i]))-1; ++j){
                    if (ofRandom(1) > noiseThresh) ofDrawRectangle(i, j, 1, 1);
                }
            }
        
             
            for (size_t i = 0; i < nProfiles+1; ++i){
                ofSetColor(profileColor[i], index*255);
                ofDrawRectangle(i, 0, 1, numLedsPerStrip*(index)*profileCounts[i]);
            }
             

            brightness = animationBrightness;
            break;
            
        case QUESTION:
            ofSetColor(loaderLColor);
            ofDrawRectangle(0, 0, numStrips/2,numLedsPerStrip*(1-index));
            ofSetColor(loaderRColor);
            ofDrawRectangle(numStrips/2, 0, numStrips,numLedsPerStrip*(1-index));
            brightness = animationBrightness;
            break;
            
        case FLASH:
            ofSetColor(white);
            ofDrawRectangle(0, 0, numStrips,numLedsPerStrip);
            brightness = flashBrightness;
            break;

    }
    
    ofSetColor(white);
    
    img.grabScreen(0, 0, numStrips,numLedsPerStrip);
    fbo.end();
    
    //img.update();
    //pixels = img.getPixels();
    setLEDs();
}

void LEDs::setLEDs() {
    
    //ofPixels& pixels;
    unsigned char * rawLEDs = img.getPixels().getData();;
    
    uint8_t buffer0[1], buffer1[4];
    srand(time(NULL));
    if(brightness>30)
        brightness=31;
    
#ifdef TARGET_RASPBERRY_PI
    
    int a;
    
    for(a=0; a<4; a++){
        buffer0[0]=0b00000000;
        wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
    }
    
    for (size_t i = 0; i < numLedsPerStrip; ++i){
        for (size_t j = 0; j < numStrips; ++j){
            if (j%2) a = j*numLedsPerStrip+i;
            else a = j*numLedsPerStrip + numLedsPerStrip-1-i;
                
            buffer1[0]=(uint8_t(brightness) & 0b00011111) | 0b11100000;
            buffer1[1]=(uint8_t)LEDs[a*4+2];  //green
            buffer1[2]=(uint8_t)LEDs[a*4+1];  //blue
            buffer1[3]=(uint8_t)LEDs[a*4+0];  //red
            wiringPiSPIDataRW(0, (unsigned char*)buffer1, 4);
            
        }
    }
    
    for(a=0; a<4; a++){
        buffer0[0]=0b11111111;
        wiringPiSPIDataRW(0, (unsigned char*)buffer0, 1);
    }
    
#endif
    
}

void LEDs::exit(){
    brightness = 0;
    setLEDs();
}


void LEDs::setup_GUI(){
    LEDs.setName("LEDs");
    
    LEDs.add(numLedsPerStrip.set ("nb de LEDs par strip", 30, 0, 60));
    LEDs.add(numStrips.set("nb de strips", 8, 0, 24));
    LEDs.add(animationBrightness.set ("lum animation", 20, 0, 31));
    LEDs.add(flashBrightness.set("lum flash", 31, 0, 31));
    
    LEDs.add(loaderLColor.set("couleur loader G", {255, 255, 0, 255}));
    LEDs.add(loaderRColor.set("couleur loader D", {0, 0, 255, 255}));
    
    LEDs.add(profileColor[0].set("couleur profil 1", {255, 255, 255}));
    LEDs.add(profileColor[1].set("couleur profil 2", {255, 255, 255}));
    LEDs.add(profileColor[2].set("couleur profil 3", {255, 255, 255}));
    LEDs.add(profileColor[3].set("couleur profil 4", {255, 255, 255}));
    LEDs.add(profileColor[4].set("couleur profil 5", {255, 255, 255}));
    LEDs.add(profileColor[5].set("couleur profil 6", {255, 255, 255}));
    LEDs.add(profileColor[6].set("couleur profil 7", {255, 255, 255}));
    
    LEDs.add(profileColor[7].set("couleur total", {255, 255, 255, 255}));
    
    LEDs.add(draw.set("draw LEDs on screen", 0));
    drawCoords.setName("visualisation LEDs");
    drawCoords.add(X.set("position X", 1800, 0, ofGetWidth()));
    drawCoords.add(Y.set("position Y", 300, 0, ofGetHeight()));
    drawCoords.add(W.set("taille X", 100, 0, ofGetWidth()));
    drawCoords.add(H.set("taille Y", 400, 0, ofGetHeight()));
    
    LEDs.add(drawCoords);
    
}
