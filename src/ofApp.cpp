#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    
    //___________________________
    // Camera setup
    
    vector<ofVideoDevice> devices = cams[0].listDevices();
    ofLog() << "Cameras: " ;
    for (auto& c : devices) {
        ofLog() << c.deviceName << " / ID: " << c.id;
        nCams++;
    }
    nCams = devices.size();
    ofLog() << "nombre de caméras: " << nCams;
    
    //___________________________
    // Setings + GUI setup
    
    setupGUI();
    
    
    //___________________________
    // Loading Background Images:
    
    /*
     for (size_t i = 0; i < ST_NR; ++i){
     switch(i){
     case QUESTION:
     for (size_t j = 0; j < nQuestions; ++j){
     ofLoadImage(questions[j], ("/data/BG/"+backgroundFiles[QUESTION]+to_string(j+1)+".png"));
     ofLog() << ("/data/BG/"+backgroundFiles[QUESTION]+to_string(j+1)+".png");
     }
     break;
     
     case PROFILE:
     for (size_t j = 0; j < nProfiles; ++j){
     ofLoadImage(profiles[j], ("/data/BG/"+backgroundFiles[PROFILE]+to_string(j+1)+".png"));
     ofLog() << ("/data/BG/"+backgroundFiles[PROFILE]+to_string(j+1)+".png");
     }
     break;
     
     case FRAME:
     for (size_t j = 0; j < nProfiles; ++j){
     ofLoadImage(frames[j], ("/data/BG/"+backgroundFiles[FRAME]+to_string(j+1)+".png"));
     ofLog() << ("/data/BG/"+backgroundFiles[FRAME]+to_string(j+1)+".png");
     }
     break;
     
     case COUNTDOWN:
     for (size_t j = 0; j < nCountdown; ++j){
     ofLoadImage(countdowns[j], ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png"));
     ofLog() << ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
     }
     break;
     
     default:
     ofLoadImage(backgrounds[i], ("/data/BG/"+backgroundFiles[i]));
     ofLog() << ("/data/BG/"+backgroundFiles[i]);
     break;
     }
     }
     */
    
    for (size_t j = 0; j < nCountdown; ++j){
        ofLoadImage(countdowns[j], ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png"));
        ofLog() << ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
    }
    
    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[INIT]));
    textureToken=!textureToken;
    
    ofEnableAlphaBlending();
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    PBtimer++;
    switch (currentState) {
        case INIT: {
            if (PBtimer==1) {
                ofLog() << "INIT";
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[STANDBY]));
                textureToken=!textureToken;
            }
            if (buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = STANDBY;
                PBtimer = 0;
                loadCSV();
                ofLog() << "CSV loaded";
                setupCams();
                ofSerialize(settings,parameters);
                settings.save("/data/settings.xml");
            }
            
            break;
        }
        case STANDBY: {
            if (PBtimer==1) {
                ofLog() << "STANDBY";
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[WELCOME]));
                textureToken=!textureToken;
            }
            if (buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = WELCOME;
                PBtimer = 0;
                // reset profile accounts
                for (size_t j = 0; j < nProfiles; ++j) {
                    profileCounts[j] = 0;
                }
            }
            break;
        }
        case WELCOME: {
            if (PBtimer==1) {
                ofLog() << "WELCOME";
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[EXPLAIN]));
                textureToken=!textureToken;
            }
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = EXPLAIN;
                PBtimer = 0;
            }
            break;
        }
        case EXPLAIN: {
            if (PBtimer==1) {
                ofLog() << "EXPLAIN";
            } else if (PBtimer==2) {
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[QUESTION]+to_string(currentQuestion+1)+".png"));
                textureToken=!textureToken;
            }
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = QUESTION;
                PBtimer = 0;
            }
            break;
        }
        case QUESTION: {
            if (PBtimer==1) {
                ofLog() << "QUESTION #" << currentQuestion+1;
            } else if (PBtimer==2) {
                //currentQuestion++;
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[QUESTION]+to_string(currentQuestion+1)+".png"));
                //textureToken=!textureToken;
            }
            
            if (PBtimer>questionTimer) {
                resetButtons();
                PBtimer = 0;
                if (currentQuestion<nQuestions-1) {
                    currentQuestion++;
                    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[QUESTION]+to_string(currentQuestion+1)+".png"));
                    textureToken=!textureToken;
                    ofLog() << "No Choice for Question: " << currentQuestion;
                } else {
                    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[COMPILING]));
                    textureToken=!textureToken;
                    currentQuestion = 0;
                    currentState = COMPILING;
                }
            } else if (buttonLPressed) {
                resetButtons();
                score = float(100-100*PBtimer/questionTimer);
                ofLog() << "Choice A for Question: " << currentQuestion+1 << " with Score: " << score << "%";
                for (size_t i = 0; i<nProfiles; ++i){
                    profileCounts[i]+=score*weightsL[currentQuestion][i];
                }
                PBtimer = 0;
                if (currentQuestion<nQuestions-1) {
                    currentQuestion++;
                    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[QUESTION]+to_string(currentQuestion+1)+".png"));
                    textureToken=!textureToken;
                } else {
                    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[COMPILING]));
                    textureToken=!textureToken;
                    currentQuestion = 0;
                    currentState = COMPILING;
                }
            } else if (buttonRPressed) {
                resetButtons();
                score = float(100-100*PBtimer/questionTimer);
                ofLog() << "Choice B for Question: " << currentQuestion+1 << " with Score: " <<  score << "%";
                for (size_t i = 0; i<nProfiles; ++i){
                    profileCounts[i]+=score*weightsR[currentQuestion][i];
                }
                PBtimer = 0;
                if (currentQuestion<nQuestions-1) {
                    currentQuestion++;
                    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[QUESTION]+to_string(currentQuestion+1)+".png"));
                    textureToken=!textureToken;
                } else {
                    ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[COMPILING]));
                    textureToken=!textureToken;
                    currentQuestion = 0;
                    currentState = COMPILING;
                }
            }
            break;
        }
        case COMPILING: {
            if (PBtimer==1) {
                ofLog() << "COMPILING";
                float max=0;
                for (size_t i =0; i< nProfiles;++i){
                    ofLog() << "compte pour profil " << profileNames[i] << " : " << profileCounts[i];
                    if (profileCounts[i]>max) {
                        max=profileCounts[i];
                        currentProfile = i;
                    }
                }
                ofLog() << "Profil choisi: " << profileNames[currentProfile];
            } else if (PBtimer==2) {
                ofLoadImage(profile, ("/data/BG/"+backgroundFiles[PROFILE]+to_string(currentProfile+1)+".png"));
                ofLoadImage(frame,   ("/data/BG/"+backgroundFiles[FRAME]+to_string(currentProfile+1)+".png"));
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[CAM_CHOICE]));
                textureToken=!textureToken;
            } else if (PBtimer>compileTimer){
                currentState = PROFILE;
                resetButtons();
                PBtimer = 0;
            }
            break;
        }
        case PROFILE: {
            if (PBtimer==1) {
                ofLog() << "PROFILE #" << currentProfile+1;
            }
            if (PBtimer>profileTimer || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = CAM_CHOICE;
                PBtimer = 0;
            }
            break;
        }
        case CAM_CHOICE: {
            if (PBtimer==1) {
                ofLog() << "CAM_CHOICE";
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[RESULT]));
                textureToken=!textureToken;
            }
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
                textureToken=!textureToken;
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
                resetButtons();
                PBtimer = 0;
                if (currentCountdown==5){
                    currentCountdown=0;
                    currentState = FLASH;
                }
            }
            break;
        }
        case FLASH: {
            if (PBtimer==1) {
                ofLog() << "FLASH";
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[RESULT]));
                textureToken=!textureToken;
            }
            if (PBtimer>flashTimer){
                resetButtons();
                currentState = RESULT;
                PBtimer = 0;
            }
            break;
        }
        case RESULT: {
            if (PBtimer==1) {
                ofLog() << "RESULT";
            } else if (PBtimer==2) {
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[PRINTING]));
                textureToken=!textureToken;
            }
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
            if (PBtimer==1) {
                ofLog() << "PRINTING";
            } else if (PBtimer==2) {
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[BYE]));
                textureToken=!textureToken;
            } else if (PBtimer==3) {
                
                ///TODO: do the blasted actual printing
                
                string fileName = eventName;
                fileName+='-'+ofToString(year)+'-'+ofToString(month)+'-'+ofToString(day)
                +'-'+ofToString(hour)+'h'+ofToString(min)+'-';
                fileName+=profileNames[currentProfile]+".png";
                ofLog() << "Photo saved as: " << fileName;
                result.save(photoPath+fileName);
            } else if (PBtimer>printingTimer){
                resetButtons();
                currentState = BYE;
                PBtimer = 0;
            }
            break;
        }
        case BYE: {
            if (PBtimer==1) {
                ofLog() << "BYE";
            } else if (PBtimer==2) {
                ofLoadImage(buffer[textureToken], ("/data/BG/"+backgroundFiles[STANDBY]));
                textureToken=!textureToken;
            }
            if (PBtimer>mainTimer || buttonLPressed || buttonRPressed){
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
            buffer[textureToken].draw(0,0);
            ofShowCursor();
            gui.draw();
            break;
        }
        case STANDBY: {
            ofHideCursor();
            buffer[textureToken].draw(0,0);
            break;
        }
        case WELCOME: {
            buffer[textureToken].draw(0,0);
            break;
        }
        case EXPLAIN: {
            buffer[textureToken].draw(0,0);
            break;
        }
        case QUESTION: {
            buffer[textureToken].draw(0,0);
            break;
        }
        case COMPILING: {
            buffer[textureToken].draw(0,0);
            break;
        }
        case PROFILE: {
            profile.draw(0,0);
            break;
        }
        case CAM_CHOICE: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            cams[!currentCam].draw(posSecCamX, posSecCamY, sizeSecCamX, sizeSecCamY);
            buffer[textureToken].draw(0,0);
            break;
        }
        case FRAME: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0);
            break;
        }
        case COUNTDOWN: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0);
            countdowns[nCountdown-1-currentCountdown].draw(860, 200, 200,200);
            break;
        }
        case FLASH: {
            cams[currentCam].draw(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0);
            if (PBtimer == 1){
                result.grabScreen(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            }
            break;
        }
        case RESULT: {
            buffer[textureToken].draw(0,0);
            result.draw(posResCamX, posResCamY, sizeResCamX, sizeResCamY);
            break;
        }
        case PRINTING: {
            buffer[textureToken].draw(0,0);
            break;
        }
        case BYE: {
            buffer[textureToken].draw(0,0);
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
    if (key == 13) {
        GUIhide = !GUIhide;
        if (GUIhide) ofHideCursor();
        else         ofShowCursor();
    }
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
    if(key=='s'){
        ofSerialize(settings,parameters);
        settings.save("/data/settings.xml");
    }
    if(key=='l'){
        settings.load("/data/settings.xml");
        ofDeserialize(settings, parameters);
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


//--------------------------------------------------------------
void ofApp::setupGUI(){
    
    // GUI/settings setup:
    
    Poco::Timestamp now;
    Poco::LocalDateTime nowLocal(now);
    
    std::string timeNow = ofxTime::Utils::format(nowLocal, Poco::DateTimeFormat::ISO8601_FORMAT);
    
    ofLog() << "Time NOW !!! : " << timeNow;
    
    parameters.setName("Reglages");
    
    event.setName("Evenement");
    event.add(eventName.set("Nom", "VYV-solidarites19"));
    event.add(year.set("Annee", ofFromString<int>(timeNow.substr(0,4)), 2019, 2029));
    event.add(month.set("Mois", ofFromString<int>(timeNow.substr(5,2)), 1, 12));
    event.add(day.set("Jour", ofFromString<int>(timeNow.substr(7,2)), 1, 31));
    event.add(hour.set("Heure", ofFromString<int>(timeNow.substr(9,2)), 0, 23));
    event.add(min.set("Minute", ofFromString<int>(timeNow.substr(11,2)), 0, 59));
    //
    parameters.add(event);
    
    files.setName("Fichiers");
    //
    files.add(logToFile.set("Log/fichier", 1));
    //
    questionsFile.setName("Questions");
    questionsFile.add(weightsFilePath.set("Chemin", "questions+.csv"));
    questionsFile.add(weightsCSVcolOffset.set("Col offset", 2, 0, 10));
    //
    files.add(questionsFile);
    parameters.add(files);
    
    coords.setName("Coordonnees displays");
    // coordonnées du cadre principal
    main.setName("Principal");
    main.add(posMainCamX.set("position  X", 493, 0, ofGetWidth()));
    main.add(posMainCamY.set("position Y", 73, 0, ofGetHeight()));
    main.add(sizeMainCamX.set("taille X", 934, 0, ofGetWidth()));
    main.add(sizeMainCamY.set("taille Y", 934, 0, ofGetHeight()));
    // coordonnées du cadre secondaire
    sec.setName("Secondaire");
    sec.add(posSecCamX.set("position X", 1500, 0, ofGetWidth()));
    sec.add(posSecCamY.set("position Y", 677, 0, ofGetHeight()));
    sec.add(sizeSecCamX.set("taille X", 330, 0, ofGetWidth()));
    sec.add(sizeSecCamY.set("taille Y", 330, 0, ofGetHeight()));
    // coordonnées du cadre résultat
    res.setName("Resultat");
    res.add(posResCamX.set("position X", 600, 0, ofGetWidth()));
    res.add(posResCamY.set("position Y", 126, 0, ofGetHeight()));
    res.add(sizeResCamX.set("taille X", 720, 0, ofGetWidth()));
    res.add(sizeResCamY.set("taille Y", 720, 0, ofGetHeight()));
    //
    coords.add(main);
    coords.add(sec);
    coords.add(res);
    parameters.add(coords);
    
    cameras.setName("Cameras");
    // Choix des caméras
    cameras.add(cam1Device.set("Camera 1", 0, 0, nCams));
    cameras.add(cam2Device.set("Camera 2", 1, 0, nCams));
    // Dimensions des caméras
    cameras.add(cam1Width.set ("Camera 1 largeur", 1920, 0, maxCamW));
    cameras.add(cam1Height.set("Camera 1 hauteur", 1080, 0, maxCamH));
    cameras.add(cam2Width.set ("Camera 2 largeur", 1920, 0, maxCamW));
    cameras.add(cam2Height.set("Camera 2 hauteur", 1080, 0, maxCamH));
    //
    parameters.add(cameras);
    
    timers.setName("Timers");
    //
    timers.add(mainTimer.set("Defaut",  120, 0, maxTimer));
    timers.add(compileTimer.set("Compilation",  120, 0, maxTimer));
    timers.add(profileTimer.set("profil",  120, 0, maxTimer));
    timers.add(flashTimer.set("flash",  120, 0, maxTimer));
    timers.add(countDownTimer.set("countdown",  30, 0, maxTimer));
    timers.add(printingTimer.set("Print",  120, 0, maxTimer));
    timers.add(questionTimer.set("question",  133, 0, maxTimer));
    timers.add(antibounceTimer.set("Anti-rebonds",  30, 0, maxTimer));
    //
    parameters.add(timers);
    
    gui.setup(parameters);
    
    gui.loadFromFile("/data/settings.xml");
    
    settings.load("/data/settings.xml");
    ofDeserialize(settings, parameters);
    
    year = ofFromString<int>(timeNow.substr(0,4));
    month = ofFromString<int>(timeNow.substr(5,2));
    day = ofFromString<int>(timeNow.substr(8,2));
    hour = ofFromString<int>(timeNow.substr(11,2));
    min =ofFromString<int>(timeNow.substr(14,2));
    
    ofLog() << year << " " << month << " " << day << " " << hour << " " << min;
    
}

//--------------------------------------------------------------
void ofApp::loadCSV(){
    
    if (logToFile){
        string logPath = eventName;
        ofLogToFile("/data/logs/"+logPath+".txt", true);
    }
    
    // Load a CSV File for profiles weights for questions
    string csvPath = weightsFilePath;
    csv.loadFile(ofToDataPath("/data/"+csvPath));
    ofLog() << '\n';
    
    for(int j=0; j<nProfiles; j++) {
        profileNames[j] = csv.data[0][j+weightsCSVcolOffset];
    }
    // Print out all rows and cols.
    for(int i=0; i<nQuestions; i++) {
        ofLog() << "Question #"  << i+1 << " :  " << '\t' << '\t'  << csv.data[i*2+1][1] << " / " << csv.data[i*2+2][1] << " : ";
        for(int j=0; j<nProfiles; j++) {
            ofLog() << "-> " <<
            tabText(profileNames[j],17) <<'\t' << '\t' <<
            //profileNames[j] <<'\t' << '\t' <<
            (weightsL[i][j] = ofFromString<int>(csv.data[i*2+1][j+weightsCSVcolOffset]))
            << " __ ou __ " <<
            (weightsR[i][j] = ofFromString<int>(csv.data[i*2+2][j+weightsCSVcolOffset])) ;
        }
        ofLog() << '\n';
    }
}

//--------------------------------------------------------------
void ofApp::setupCams(){
    
    cams[0].setDeviceID(cam1Device);
    cams[0].setDesiredFrameRate(30);
    cams[0].setPixelFormat(OF_PIXELS_NATIVE);
    cams[0].setup(cam1Width, cam1Height);
    
    cams[1].setDeviceID(cam2Device);
    cams[1].setDesiredFrameRate(30);
    cams[1].setPixelFormat(OF_PIXELS_NATIVE);
    cams[1].setup(cam2Width, cam2Height);
    
    ofLog() << "size 1: " << cams[0].getWidth() << " / " << cams[0].getHeight();
    ofLog() << "size 2: " << cams[1].getWidth() << " / " << cams[1].getHeight();
}

//--------------------------------------------------------------
string ofApp::tabText(string& s, int offset){
    string result = s;
    int spaces = offset-s.size();
    for (size_t i = 0; i< spaces; ++i) {
        result+=' ';
    }
    result+= " : ";
    return result;
}
