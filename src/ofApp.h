#pragma once

// addons
#include "ofxTime.h"
#include "ofxCsv.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"
#ifdef TARGET_RASPBERRY_PI
#include "ofxGPIO.h"
#endif

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
    posRCamX{1000}, posRCamY{677},
    sizeRCamX{640}, sizeRCamY{360};

    ofParameterGroup res;
    // coordonnées du cadre résultats
    ofParameter<int>
    posResCamX{1000}, posResCamY{677},
    sizeResCamX{640}, sizeResCamY{360};

    ofParameterGroup countdown;
    // coordonnées du countdown
    ofParameter<int>
    posCDX{860}, posCDY{200},
    sizeCDX{200}, sizeCDY{200};

    ofParameterGroup timerPos;
    // coordonnées du countdown
    ofParameter<int>
    posTimerX{860}, posTimerY{200},
    sizeTimerX{200}, sizeTimerY{200};

    ofParameterGroup tickPar;
    // coordonnées du cadre résultats
    ofParameter<int>
    ticketMarginXLeft{10}, ticketMarginXRight{10},
    ticketMarginYTop{10},  ticketMarginYBottom{10},
    sizeTktX{635}, sizeTktY{635},
    profilMargin{20}, profilSizeY{180}, textFontSize{30};
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
                        frameTimer{120},
                        questionTimer{133};

    ofParameter<bool> drawFps;


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
    void flash(bool on);
    void drawTimer(int timer);
    void ledButtons(bool L, bool R);
    void getButtons();
    void resetButtons();
    void setupGUI();
    void loadCSV();
    string tabText(string& s, int offset);


    //______________________
    // Components:

    ofxPanel gui;
    ofParameterGroup parameters;
    bool GUIhide{0};
    ofXml settings;

    Cameras cams;

    Background bg;

    int ticketWidth, ticketHeight;

    ofFbo fbo;
    int fboMaxSizeX{1000},fboMaxSizeY{1000};

    ofTrueTypeFont font;

    LEDs leds;

    #ifdef TARGET_RASPBERRY_PI
    GPIO buttonL, buttonR, ledL, ledR, flashRelay;
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

    static const int nTimer{12};

    int resultCount{0};

    ofTexture profile;
    ofTexture frame;
    ofTexture profilTicket;
    ofTexture countdowns[nCountdown];
    ofTexture timerV[nTimer];

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

    ofxCsv csv;

    int weightsL[nQuestions][nProfiles];
    int weightsR[nQuestions][nProfiles];

    float score;
    float profileCounts[nProfiles];
    int   profileScores[nProfiles];

    std::string timeStart;
    std::string timeNow;
    std::string timeSet;

    Poco::DateTime setTime;
    ofxTime::Period timeOffset;

};
