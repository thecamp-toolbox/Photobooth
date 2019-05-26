#pragma once

// addons
#include "ofxTime.h"
#include "ofxCsv.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"

// Components
#include "Cameras.hpp"
#include "Background.hpp"
#include "LEDs.hpp"


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
    ofParameter<bool> print{false};
    
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
    
    ofParameterGroup choiceL;
    // coordonnées du cadre de choix gauche
    ofParameter<int>
    posLCamX{1500}, posLCamY{677},
    sizeLCamX{330}, sizeLCamY{330};
    ofParameterGroup choiceR;
    // coordonnées du cadre de choix gauche
    ofParameter<int>
    posRCamX{1500}, posRCamY{677},
    sizeRCamX{330}, sizeRCamY{330};

    ofParameterGroup res;
    // coordonnées du cadre résultats
    ofParameter<int>
    posResCamX{1500}, posResCamY{677},
    sizeResCamX{330}, sizeResCamY{330};
    
    ofParameterGroup tickPar;
    // coordonnées du cadre résultats
    ofParameter<int>
    ticketMarginXLeft{10}, ticketMarginXRight{10},
    ticketMarginYTop{10},  ticketMarginYBottom{10},
    sizeTktX{635}, sizeTktY{635},
    textMargin{20}, textFontSize{15};
    ofParameter<string> fontName {"sans-serif"};
    
    ofParameterGroup timers;
    ofParameter<int> frameRate;
    // Timers:
    ofParameter<float>  mainTimer{120},
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
    string tabText(string& s, int offset);
    
    
    //______________________
    // Components:
    
    ofxPanel gui;
    ofParameterGroup parameters;
    bool GUIhide{1};
    ofXml settings;
    
    Cameras cams;
    
    Background bg;
    
    int ticketWidth, ticketHeight;
    
    ofFbo fbo;
    int fboMaxSizeX{1000},fboMaxSizeY{1000};
    
    ofTrueTypeFont font;
    
    LEDs leds;
    
    #ifdef TARGET_RASPBERRY_PI
    GPIO buttonL, buttonR;
    #endif
    
    enum States {
        INIT,
        STANDBY,
        WELCOME,
        EXPLAIN,
        QUESTION,
        COMPILING,
        PROFILE,
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
        "result",
        "printing.png",
        "bye.png"
    };
    
    static const int nQuestions{10};
    int currentQuestion {0};
 
    static const int nProfiles{7};
    string profileNames[nProfiles]{
        "agrikool",
        "ecowarrior",
        "sportify",
        "educulteur",
        "gik2point0",
        "habitateur",
        "essejivé"
    };
    int currentProfile {0};
    
    static const int nCountdown{5};
    int currentCountdown{0};
    
    int resultCount{0};
    
    ofTexture profile;
    ofTexture frame;
    //ofTexture backgrounds[ST_NR];
    //ofTexture questions[nQuestions];
    //ofTexture profiles[nProfiles];
    //ofTexture frames[nProfiles];
    ofTexture countdowns[nCountdown];
    
    int PBtimer{0};
    int maxTimer{20};
    
    ofImage result, ticket;
    
    bool buttonLPressed{0},
         buttonRPressed{0},
         buttonLreleased{0},
         buttonRreleased{0},
         keyLreleased{1},
         keyRreleased{1};
    
    string photoPath {"/data/photos/"};
    
    wng::ofxCsv csv;
    
    int weightsL[nQuestions][nProfiles];
    int weightsR[nQuestions][nProfiles];
    
    float score;
    float profileCounts[nProfiles];
    int   profileScores[nProfiles];

    
};
