#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
public:
    
//---------------------------------------------------------
// User-defined variables:
    
    ofParameterGroup event;
    ofParameter<string> eventName;
    ofParameter<int>
    year, month, day, hour, min;
    
    ofParameterGroup files;
    // Other stuff:
    ofParameter<bool> logToFile{false}; // set this to true to write the Log to a file
    
    ofParameterGroup questionsFile;
    // CSV file for questions weights
    ofParameter<string> weightsFilePath {"questions.csv"};
    ofParameter<int> weightsCSVcolOffset {2}; // the number of cols to offset in the  spreadsheet
    
    ofParameterGroup coords;
    // coordonnées du cadre principal
    ofParameterGroup main;
    ofParameter<int>
    posMainCamX{493}, posMainCamY{73},
    sizeMainCamX{934}, sizeMainCamY{934};
    ofParameterGroup sec;
    // coordonnées du cadre secondaire
    ofParameter<int>
    posSecCamX{1500}, posSecCamY{677},
    sizeSecCamX{330}, sizeSecCamY{330};
    
    ofParameterGroup cameras;
    // Tailes des caméras:
    ofParameter<int> cam1Device{0},   cam2Device{1},
                     cam1Width{1280}, cam1Height{720},
                     cam2Width{1280}, cam2Height{720};
    
    ofParameterGroup timers;
    ofParameter<int> frameRate;
    // Timers:
    ofParameter<float>  antibounceTimer{30},
                        mainTimer{120},
                        compileTimer{120},
                        profileTimer{120},
                        flashTimer{120},
                        countDownTimer{30},
                        printingTimer{120},
                        questionTimer{133};
    
    
  
//---------------------------------------------------------
    
    // Methods:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
    void getButtons();
    void resetButtons();
    void setupGUI();
    void loadCSV();
    void setupCams();
    string tabText(string& s, int offset);
    
    
    //______________________
    // Components:
    
    ofxPanel gui;
    ofParameterGroup parameters;
    bool GUIhide{1};
    
    ofVideoGrabber cams[2];
    bool currentCam{0};
    int nCams{0};
    
    int maxCamW{1920},
        maxCamH{1080};
    
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
    
    string profileNames[PR_NR]{
        "agrikool",
        "ecowarrior",
        "sportify",
        "educulteur",
        "gik2point0",
        "habitateur",
        "essejivé"
    };
    
    static const int nCountdown{5};
    int currentCountdown{0};
    
    ofImage backgrounds[ST_NR];
    ofImage questions[nQuestions];
    ofImage profiles[PR_NR];
    ofImage frames[PR_NR];
    ofImage countdowns[nCountdown];
    
    int PBtimer{0};
    int maxTimer{1000};
    
    ofImage result;
    
    bool buttonLPressed{0},
         buttonRPressed{0},
         keyLreleased{1},
         keyRreleased{1};
    
    string photoPath {"/data/photos/"};
    
    wng::ofxCsv csv;
    
    int weightsL[nQuestions][PR_NR];
    int weightsR[nQuestions][PR_NR];
    
    float score;
    float profileCounts[PR_NR];

    
};
