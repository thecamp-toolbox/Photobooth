#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
    
    ofVideoGrabber* cams;
    int camWidth{1920}, camHeight{1080};
    
    
    enum States {
        INIT,
        STANDBY,
        WELCOME,
        EXPLAIN,
        QUESTION/*1,
        QUESTION2,
        QUESTION3,
        QUESTION4,
        QUESTION5,
        QUESTION6,
        QUESTION7,
        QUESTION8,
        QUESTION9,
        QUESTION10*/,
        COMPILING,
        PROFILE/*1,
        PROFILE2,
        PROFILE3,
        PROFILE4,
        PROFILE5,
        PROFILE6,
        PROFILE7*/,
        CAM_CHOICE,
        FRAME,
        COUNTDOWN,
        FLASH,
        RESULT,
        PRINTING,
        BYE,
        END
    } currentState{};
    
    enum Profiles {
        AGRI,
        ECO,
        SPORT,
        EDUCULT,
        GEEK,
        HOMER,
        SJW
    } profile{};
    
    
    
    
};
