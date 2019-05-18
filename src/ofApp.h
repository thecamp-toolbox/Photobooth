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
    
    
    ofVideoGrabber cams[2];
    const int camWidth{1920}, camHeight{1080};
    bool currentCam{0};
    
    // coordonnées du cadre principal
    const int
        posMainCamX{493}, posMainCamY{73},
        sizeMainCamX{934}, sizeMainCamY{934},
    // coordonnées du cadre secondaire
        posSecCamX{1500}, posSecCamY{677},
        sizeSecCamX{330}, sizeSecCamY{330};
    
    
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
    } currentState{CAM_CHOICE};
    
    string backgroundFiles[States::END]
    {
        "init.png",
        "standby.png",
        "welcome.png",
        "explain.png",
        "question",
        "compiling.png",
        "profile",
        "cam_choice.png",
        "frame",
        "countdown",
        "flash.png",
        "result.png",
        "printing.png",
        "bye.png"
    };
    
    static const int nQuestions{10};
    
    enum Profiles {
        AGRI,
        ECO,
        SPORT,
        EDUCULT,
        GEEK,
        HOMER,
        SJW,
        NR
    } profile{};
    
    static const int nImages =
                        States::END
                        +nQuestions-1
                        +(Profiles::NR-1)*2;
    
    ofImage backgrounds[nImages];
    
    
    
    
};
