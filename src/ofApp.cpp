#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    //___________________________
    // Camera setup
    
    // uncomment to get a list of connected cams:
    /*
    vector<ofVideoDevice> devices = cams[0].listDevices();
    ofLog() << "Cameras: " ;
    for (auto& c : devices) {
        ofLog() << c.deviceName << " / ID: " << c.id;
    }
    */
    
    cams[0].setDeviceID(0);
    cams[0].setDesiredFrameRate(30);
    cams[0].setPixelFormat(OF_PIXELS_NATIVE);
    cams[0].setup(cam1Width, cam1Height);
    
    cams[1].setDeviceID(1);
    cams[1].setDesiredFrameRate(30);
    cams[1].setPixelFormat(OF_PIXELS_NATIVE);
    cams[1].setup(cam2Width, cam2Height);
    
    ofLog() << "size 1: " << cams[0].getWidth() << " / " << cams[0].getHeight();
    ofLog() << "size 2: " << cams[1].getWidth() << " / " << cams[1].getHeight();
    
    //___________________________
    // Loading Background Images:
    for (size_t i = 0; i < ST_NR; ++i){
        switch(i){
            case QUESTION:
                for (size_t j = 0; j < nQuestions; ++j){
                    questions[j].load("/data/BG/"+backgroundFiles[QUESTION]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[QUESTION]+to_string(j+1)+".png");
                }
                break;
                
            case PROFILE:
                for (size_t j = 0; j < PR_NR; ++j){
                    profiles[j].load("/data/BG/"+backgroundFiles[PROFILE]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[PROFILE]+to_string(j+1)+".png");
                }
                break;
                
            case FRAME:
                for (size_t j = 0; j < PR_NR; ++j){
                    frames[j].load("/data/BG/"+backgroundFiles[FRAME]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[FRAME]+to_string(j+1)+".png");
                }
                break;
            
            case COUNTDOWN:
                for (size_t j = 0; j < nCountdown; ++j){
                    countdowns[j].load("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
                    ofLog() << ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
                }
                break;
            
            default:
                backgrounds[i].load("/data/BG/"+backgroundFiles[i]);
                ofLog() << ("/data/BG/"+backgroundFiles[i]);
                break;
        }
    }
    
    //___________________________
    //
    ofEnableAlphaBlending();
    
    ///TODO: save to a different file tagged with event+date+time
    if (logToFile) ofLogToFile("/data/logs/myLogFile.txt", true);
    

    // Load a CSV File for profiles weights for questions
    csv.loadFile(ofToDataPath("/data/"+weightsFilePath));
    
    // Print out all rows and cols.
    for(int i=0; i<nQuestions; i++) {
        ofLog() << "Question #" << i+1 << " :" << '\t' << csv.data[i*2][1] << " / " << csv.data[i*2+1][1] << " : ";
        for(int j=0; j<PR_NR; j++) {
            ofLog() << "-> " <<
            profileNames[j] <<  " : " <<'\t' << '\t' <<
                (weightsL[i*2][j] = ofFromString<int>(csv.data[i*2][j+weightsCSVcolOffset]))
            << " __ ou __ " <<
                (weightsR[i*2+1][j] = ofFromString<int>(csv.data[i*2+1][j+weightsCSVcolOffset])) ;
        }
        ofLog() << '\n';
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    PBtimer++;
    switch (currentState) {
        case INIT: {
            if (PBtimer==1) ofLog() << "INIT";
            
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = STANDBY;
                PBtimer = 0;
            }
            break;
        }
        case STANDBY: {
            if (PBtimer==1) ofLog() << "STANDBY";
            
            if (buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = WELCOME;
                PBtimer = 0;
                // reset profile accounts
                for (size_t j = 0; j < PR_NR; ++j) {
                    profileCounts[j] = 0;
                }
            }
            break;
        }
        case WELCOME: {
            if (PBtimer==1) ofLog() << "WELCOME";
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = EXPLAIN;
                PBtimer = 0;
            }
            break;
        }
        case EXPLAIN: {
            if (PBtimer==1) ofLog() << "EXPLAIN";
            
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = QUESTION;
                PBtimer = 0;
            }
            break;
        }
        case QUESTION: {
            if (PBtimer==1) ofLog() << "QUESTION #" << currentQuestion+1 ;
            
            ///TODO: parse results with timer weighting
            
            if (PBtimer>questionTimer) {
                resetButtons();
                currentQuestion++;
                PBtimer = 0;
                ofLog() << "No Choice for Question: " << currentQuestion;
            } else if (buttonLPressed) {
                resetButtons();
                score = float(100-100*PBtimer/questionTimer);
                ofLog() << "Choice A for Question: " << currentQuestion+1 << " with Score: " << score << "%";
                for (size_t i = 0; i<PR_NR; ++i){
                    profileCounts[i]+=score*weightsL[currentQuestion][i];
                }
                currentQuestion++;
                PBtimer = 0;
            } else if (buttonRPressed) {
                resetButtons();
                score = float(100-100*PBtimer/questionTimer);
                ofLog() << "Choice B for Question: " << currentQuestion+1 << " with Score: " <<  score << "%";
                for (size_t i = 0; i<PR_NR; ++i){
                    profileCounts[i]+=score*weightsR[currentQuestion][i];
                }
                currentQuestion++;
                PBtimer = 0;
            }
            if (currentQuestion==nQuestions) {
                currentQuestion = 0;
                currentState = COMPILING;
            }
            
            break;
        }
        case COMPILING: {
            if (PBtimer==1) ofLog() << "COMPILING";
            
            if (PBtimer>compileTimer){
                float max=0;
                for (size_t i =0; i< PR_NR;++i){
                    ofLog() << "compte pour profil " << profileNames[i] << " : " << profileCounts[i];
                    if (profileCounts[i]>max) {
                        max=profileCounts[i];
                        profile = Profiles(i);
                    }
                }
                ofLog() << "Profil choisi: " << profileNames[profile];
                currentState = PROFILE;
                PBtimer = 0;
            }
            break;
        }
        case PROFILE: {
            if (PBtimer==1) ofLog() << "PROFILE #" << profile+1;
            
            if (PBtimer>profileTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = CAM_CHOICE;
                PBtimer = 0;
            }
            break;
        }
        case CAM_CHOICE: {
            if (PBtimer==1) ofLog() << "CAM_CHOICE";
            if (buttonRPressed) {
                resetButtons();
                currentCam = !currentCam;
            }
            cams[currentCam].update();
            cams[!currentCam].update();
            if (PBtimer>mainTimer || buttonLPressed){
                resetButtons();
                currentState = FRAME;
                PBtimer = 0;
            }
            break;
        }
        case FRAME: {
            if (PBtimer==1) ofLog() << "FRAME";
            
            cams[currentCam].update();
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = COUNTDOWN;
                PBtimer = 0;
            }
            break;
        }
        case COUNTDOWN: {
            if (PBtimer==1) ofLog() << "COUNTDOWN";
            
            cams[currentCam].update();
            if (PBtimer>countDownTimer){
                currentCountdown++;
                PBtimer = 0;
                if (currentCountdown==5){
                    currentCountdown=0;
                    currentState = FLASH;
                }
            }
            break;
        }
        case FLASH: {
            if (PBtimer==1) ofLog() << "FLASH";

            if (PBtimer>flashTimer){
                currentState = RESULT;
                PBtimer = 0;
            }
            break;
        }
        case RESULT: {
            if (PBtimer==1) ofLog() << "RESULT";
            if (buttonRPressed) {
                resetButtons();
                currentState = COUNTDOWN;
                PBtimer = 0;
            }
            if (PBtimer>mainTimer || buttonLPressed ){
                resetButtons();
                currentState = PRINTING;
                PBtimer = 0;
            }
            break;
        }
        case PRINTING: {
            if (PBtimer==1) ofLog() << "PRINTING";
            
            ///TODO: do the blasted actual printing
            
            if (PBtimer>printingTimer){
                currentState = BYE;
                PBtimer = 0;
            }
            break;
        }
        case BYE: {
            if (PBtimer==1) ofLog() << "BYE";
            
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                ///TODO: save photo with profile name and actual Time+Date + event
                string fileName = "snapshot_"+profileNames[profile]+"-"+ofToString(int(ofRandom(1000)))+".png";
                ofLog() << "Photo saved as: " << fileName;
                result.save(photoPath+fileName);
                
                resetButtons();
                currentState = STANDBY;
                PBtimer = 0;
            }
            break;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    switch (currentState) {
        case INIT: {
            backgrounds[INIT].draw(0,0);
            break;
        }
        case STANDBY: {
            backgrounds[STANDBY].draw(0,0);
            break;
        }
        case WELCOME: {
            backgrounds[WELCOME].draw(0,0);
            break;
        }
        case EXPLAIN: {
            backgrounds[EXPLAIN].draw(0,0);
            break;
        }
        case QUESTION: {
            questions[currentQuestion].draw(0,0);
            break;
        }
        case COMPILING: {
            backgrounds[COMPILING].draw(0,0);
            break;
        }
        case PROFILE: {
            profiles[profile].draw(0,0);
            break;
        }
        case CAM_CHOICE: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            cams[!currentCam].draw(posSecCamX, posSecCamY, sizeSecCamX, sizeSecCamY);
            backgrounds[CAM_CHOICE].draw(0,0);
            break;
        }
        case FRAME: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frames[profile].draw(0,0);
            break;
        }
        case COUNTDOWN: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frames[profile].draw(0,0);
            countdowns[nCountdown-1-currentCountdown].draw(860, 200, 200,200);
            break;
        }
        case FLASH: {
            //backgrounds[FLASH].draw(0,0);
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frames[profile].draw(0,0);
            if (PBtimer == 1){
                result.grabScreen(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            }
            break;
        }
        case RESULT: {
            backgrounds[RESULT].draw(0,0);
            result.draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            break;
        }
        case PRINTING: {
            backgrounds[PRINTING].draw(0,0);
            break;
        }
        case BYE: {
            backgrounds[BYE].draw(0,0);
            break;
        }
    }

}

//--------------------------------------------------------------
void ofApp::exit(){
    ofLog() << "QUITTING THE APP at ";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 13) currentCam = !currentCam;
    else if (key == 57356 && keyLreleased ) {
        buttonLPressed = 1;
        keyLreleased = 0;
        //ofLog() << "left button pressed";
    }
    else if (key == 57358 && keyRreleased ) {
        buttonRPressed = 1;
        keyRreleased = 0;
        //ofLog() << "right button pressed";
    }
    //ofLog() << key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == 57356) {keyLreleased = 1; }
    else if (key == 57358) {keyRreleased = 1; }
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::getButtons(){
    
    ///TODO: get buttons from GPIOs
    
}

//--------------------------------------------------------------
void ofApp::resetButtons(){
    buttonLPressed = 0;
    buttonRPressed = 0;
}
