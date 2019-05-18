#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    
//---------------------------------------------------------
// User-defined variables:
    
    // coordonnées du cadre principal
    const int
    posMainCamX{493}, posMainCamY{73},
    sizeMainCamX{934}, sizeMainCamY{934},
    // coordonnées du cadre secondaire
    posSecCamX{1500}, posSecCamY{677},
    sizeSecCamX{330}, sizeSecCamY{330};
    
    int maxTimer{120};
    int maxCountDownTimer{30};
    
    const int cam1Width{1280}, cam1Height{720};
    const int cam2Width{1280}, cam2Height{720};
  
//---------------------------------------------------------
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
    
    
    ofVideoGrabber cams[2];
    bool currentCam{0};
    
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
        ST_NR
    } currentState{INIT};
    
    string backgroundFiles[ST_NR]
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
    int currentQuestion {0};
    
    enum Profiles {
        AGRI,
        ECO,
        SPORT,
        EDUCULT,
        GEEK,
        HOMER,
        SJW,
        PR_NR
    } profile{};
    
    static const int nCountdown{5};
    int currentCountdown{0};
    
    ofImage backgrounds[ST_NR];
    ofImage questions[nQuestions];
    ofImage profiles[PR_NR];
    ofImage frames[PR_NR];
    ofImage countdowns[nCountdown];
    
    int PBtimer{0};
    
    ofImage result;
    
    
    
};
