#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    setupGUI();
    
    cams.setup();
    
    //___________________________
    // Loading Images:
    
    for (size_t j = 0; j < nCountdown; ++j){
        ofLoadImage(countdowns[j], ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png"));
        ofLog() << ("/data/BG/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
    }
    
    bg.load("/data/BG/"+backgroundFiles[INIT]);
    
    ofEnableAlphaBlending();
    
#ifdef TARGET_RASPBERRY_PI
    buttonL.setup("17");
    buttonL.export_gpio();
    buttonL.setdir_gpio("in");

    buttonR.setup("27");
    buttonR.export_gpio();
    buttonR.setdir_gpio("in");
    
#endif
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    PBtimer++;
    getButtons();
    switch (currentState) {
        case INIT: {
            cams.update_all();
            if (PBtimer==1) {
                bg.next();
                ofShowCursor();
                ofLog() << "INIT";
                bg.load("/data/BG/"+backgroundFiles[STANDBY]);
                leds.currentAnimation = leds.INIT;
            }
            if (buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = STANDBY;
                PBtimer = 0;
                loadCSV();
                ofLog() << "CSV loaded";
                ofSerialize(settings,parameters);
                settings.save("/data/settings.xml");
                ofHideCursor();
                ticketWidth = ticketMarginXLeft+ticketMarginXRight+sizeTktX;
                ticketHeight= ticketMarginYTop+ticketMarginYBottom+sizeTktY+textMargin+textFontSize;
                fbo.allocate(ticketWidth, ticketHeight, GL_RGBA );
                font.load( OF_TTF_SANS,textFontSize,true,true);
                leds.setup();
                leds.currentAnimation = leds.NONE;
                bg.next();
            }
     
            break;
        }
        case STANDBY: {
            if (PBtimer==1) {
                ofLog() << "STANDBY";
                bg.load("/data/BG/"+backgroundFiles[WELCOME]);
            }
            if (buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = WELCOME;
                PBtimer = 0;
                // reset profile accounts
                for (size_t j = 0; j < nProfiles; ++j) {
                    profileCounts[j] = 0;
                }
                bg.next();
            }
            break;
        }
        case WELCOME: {
            if (PBtimer==1) {
                ofLog() << "WELCOME";
                bg.load("/data/BG/"+backgroundFiles[EXPLAIN]);
            }
            if (PBtimer>mainTimer*frameRate || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = EXPLAIN;
                PBtimer = 0;
                bg.next();
            }
            break;
        }
        case EXPLAIN: {
            if (PBtimer==1) {
                ofLog() << "EXPLAIN";
                bg.load("/data/BG/" + backgroundFiles[QUESTION] + to_string(currentQuestion+1) + ".png");
            }
            if (PBtimer>mainTimer*frameRate || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = QUESTION;
                PBtimer = 0;
                bg.next();
            }
            break;
        }
        case QUESTION: {
            if (PBtimer==1) {
                ofLog() << "QUESTION #" << currentQuestion+1;
            bg.load("/data/BG/" + backgroundFiles[QUESTION] + to_string(currentQuestion+2) + ".png");
            leds.currentAnimation = leds.QUESTION;
            }
            
            leds.index = float(PBtimer)/float(questionTimer*frameRate);
            
            if (PBtimer>questionTimer*frameRate) {
                resetButtons();
                PBtimer = 0;
                bg.next();
                if (currentQuestion<nQuestions-1) {
                    currentQuestion++;
                    bg.load("/data/BG/" + backgroundFiles[QUESTION] + to_string(currentQuestion+2) + ".png");
                    ofLog() << "No Choice for Question: " << currentQuestion;
                    //bg.next();
                } else {
                    bg.load("/data/BG/"+backgroundFiles[COMPILING]);
                    bg.next();
                    currentQuestion = 0;
                    currentState = COMPILING;
                }
                
            } else if (buttonLPressed) {
                resetButtons();
                bg.next();
                score = float(100-100*PBtimer/(questionTimer*frameRate));
                ofLog() << "Choice A for Question: " << currentQuestion+1 << " with Score: " << score << "%";
                for (size_t i = 0; i<nProfiles; ++i){
                    profileCounts[i]+=score*weightsL[currentQuestion][i];
                }
                PBtimer = 0;
                if (currentQuestion<nQuestions-1) {
                    currentQuestion++;
                    bg.load("/data/BG/" + backgroundFiles[QUESTION] + to_string(currentQuestion+2) + ".png");
                    //bg.next();
                } else {
                    bg.load("/data/BG/"+backgroundFiles[COMPILING]);
                    bg.next();
                    currentQuestion = 0;
                    currentState = COMPILING;
                }
                
            } else if (buttonRPressed) {
                resetButtons();
                bg.next();
                score = float(100-100*PBtimer/(questionTimer*frameRate));
                ofLog() << "Choice B for Question: " << currentQuestion+1 << " with Score: " <<  score << "%";
                for (size_t i = 0; i<nProfiles; ++i){
                    profileCounts[i]+=score*weightsR[currentQuestion][i];
                }
                PBtimer = 0;
                if (currentQuestion<nQuestions-1) {
                    currentQuestion++;
                    bg.load("/data/BG/" + backgroundFiles[QUESTION] + to_string(currentQuestion+2) + ".png");
                    //bg.next();
                } else {
                    bg.load("/data/BG/"+backgroundFiles[COMPILING]);
                    bg.next();
                    currentQuestion = 0;
                    currentState = COMPILING;
                }
            }
            break;
        }
        case COMPILING: {
            if (PBtimer==1) {
                leds.currentAnimation = leds.COMPILE;
                //leds.blackout();
                //bg.next();
                ofLog() << "COMPILING";
                float max=0;
                for (size_t i =0; i< nProfiles;++i){
                    ofLog() << "compte pour profil " << profileNames[i] << " : " << profileCounts[i]
                    << " -> " << (profileScores[i] = floor(profileCounts[i]/111));
                    leds.profileCounts[i] = profileCounts[i]/1200.;
                    leds.profileCounts[nProfiles] += leds.profileCounts[i]/nProfiles;
                    if (profileCounts[i]>max) {
                        max=profileCounts[i];
                        currentProfile = i;
                    }
                }
                ofLog() << "Profil choisi: " << profileNames[currentProfile];
                ofLoadImage(profile, ("/data/BG/"+backgroundFiles[PROFILE]+to_string(currentProfile+1)+".png"));
                ofLoadImage(frame,   ("/data/BG/"+backgroundFiles[FRAME]+to_string(currentProfile+1)+".png"));
                bg.load("/data/BG/"+backgroundFiles[CAM_CHOICE]);
            }
            
            leds.index = float(PBtimer)/float(compileTimer*frameRate);
            
            if (PBtimer>compileTimer*frameRate){
                bg.next();
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
            if (PBtimer>profileTimer*frameRate || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = CAM_CHOICE;
                PBtimer = 0;
            }
            break;
        }
        case CAM_CHOICE: {
            cams.update_all();
            leds.currentAnimation = leds.NONE;
            
            if (PBtimer==1) {
                ofLog() << "CAM_CHOICE";
                bg.load("/data/BG/"+backgroundFiles[FLASH]);

            }
            if (buttonRPressed) {
                bg.next();
                resetButtons();
                cams.current = 0;
                currentState = FRAME;
                PBtimer = 0;
            } else if (buttonLPressed){
                bg.next();
                resetButtons();
                cams.current = 1;
                currentState = FRAME;
                PBtimer = 0;
            } else if (PBtimer>mainTimer*frameRate){
                bg.next();
                resetButtons();
                currentState = FRAME;
                PBtimer = 0;
            }
            break;
        }
        case FRAME: {
            cams.update_one();
            
            if (PBtimer==1) ofLog() << "FRAME";
            if (PBtimer>mainTimer*frameRate || buttonLPressed || buttonRPressed){
                
                resetButtons();
                currentState = COUNTDOWN;
                PBtimer = 0;
            }
            break;
        }
        case COUNTDOWN: {
            cams.update_one();
            
            if (PBtimer==1) ofLog() << "COUNTDOWN";

            if (PBtimer>countDownTimer*frameRate){
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
            cams.update_one();
            leds.currentAnimation = leds.FLASH;
            
            if (PBtimer==1) {
                ofLog() << "FLASH";
                bg.load("/data/BG/"+backgroundFiles[RESULT]+to_string(resultCount+1)+".png");
            }
            
            if (PBtimer>flashTimer*frameRate){
                bg.next();
                resetButtons();
                currentState = RESULT;
                leds.currentAnimation = leds.NONE;
                PBtimer = 0;
            }
            break;
        }
        case RESULT: {
            if (PBtimer==1) {
                ofLog() << "RESULT, #" ;
                
            }
            if (buttonRPressed) {
                resetButtons();
                resultCount++;
                if (resultCount<3){
                    bg.load("/data/BG/"+backgroundFiles[RESULT]+to_string(resultCount+1)+".png");
                    currentState = COUNTDOWN;
                } else {
                    bg.load("/data/BG/"+backgroundFiles[PRINTING]);
                    bg.next();
                    currentState = PRINTING;
                }
                PBtimer = 0;
            }
            if (PBtimer>mainTimer*frameRate || buttonLPressed){
                bg.load("/data/BG/"+backgroundFiles[PRINTING]);
                bg.next();
                resetButtons();
                currentState = PRINTING;
                PBtimer = 0;
            }
            break;
        }
        case PRINTING: {
            if (PBtimer==1) {
                ofLog() << "PRINTING";
                bg.load("/data/BG/"+backgroundFiles[BYE]);
                
                fbo.begin();
                    ofClear(255,255,255, 0);
                    ofSetColor(255,255,255, 255);
                    ofDrawRectangle(0, 0, ticketWidth, ticketHeight);
            
                    result.draw(ticketMarginXLeft, ticketMarginYTop, sizeTktX, sizeTktY);
                    ofSetColor(0);
                    font.drawString("rendez-vous sur http://vyv.app/"+profileNames[currentProfile],
                                    //ticketMarginXLeft,
                                    (sizeTktX-30-(profileNames[currentProfile].size())*textFontSize)/2,
                                    ticketMarginYTop+sizeTktY+textMargin+textFontSize);
                    ticket.grabScreen(0, 0, ticketWidth, ticketHeight);
                fbo.end();
                
                fbo.begin();
                    ticket.draw(ticketWidth, ticketHeight, -ticketWidth, -ticketHeight);
                    result.grabScreen(0, 0, ticketWidth, ticketHeight);
                fbo.end();
                //ofEnableAlphaBlending();
                
                string fileName = eventName;
                fileName+='-'+ofToString(year)+'-'+ofToString(month)+'-'+ofToString(day)
                +'-'+ofToString(hour)+'h'+ofToString(min)+'-';
                fileName+=profileNames[currentProfile]+".png";
                ofLog() << "Photo saved as: " << fileName;
                result.save(photoPath+fileName);
                
                if (print){
                    string printCommand = "lp ";
                    printCommand +=photoPath+fileName;
                    ofSystem(printCommand);
                }
            }
            
            if (PBtimer>printingTimer*frameRate){
                bg.next();
                resetButtons();
                currentState = BYE;
                PBtimer = 0;
            }
            break;
        }
        case BYE: {
            if (PBtimer==1) {
                ofLog() << "BYE";
                bg.load("/data/BG/"+backgroundFiles[STANDBY]);
                
            }
            if (PBtimer>mainTimer*frameRate || buttonLPressed || buttonRPressed){
                bg.next();
                resetButtons();
                currentState = STANDBY;
                PBtimer = 0;
            }
            break;
        }
        default:
            break;
    }
    
    //if (leds.draw)
    leds.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofClear(0, 0, 0);
    
    switch (currentState) {
        case INIT: {
            //bg.draw();
            cams.draw_all(posLCamX, posLCamY, sizeLCamX, sizeLCamY, posRCamX, posRCamY, sizeRCamX, sizeRCamY);
            gui.draw();
            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);
            break;
        }
        case QUESTION:{
            bg.draw();
            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);
            break;
        }
        case COMPILING:{
            bg.draw();
            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);
            break;
        }
        case PROFILE: {
            profile.draw(0,0);
            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);
            break;
        }
        case CAM_CHOICE: {
            cams.draw_all(posLCamX, posLCamY, sizeLCamX, sizeLCamY, posRCamX, posRCamY, sizeRCamX, sizeRCamY);
             bg.draw();
            break;
        }
        case FRAME: {
            cams.draw_one(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0);
            break;
        }
        case COUNTDOWN: {
            cams.draw_one(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0);
            countdowns[nCountdown-1-currentCountdown].draw(860, 200, 200,200);
            break;
        }
        case FLASH: {
            //bg.draw();
            cams.draw_one(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0);
            //buffer[textureToken].draw(0,0);
            if (PBtimer == 1){
                result.grabScreen(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            }
            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);
            break;
        }
        case RESULT: {
            bg.draw();
            result.draw(posResCamX, posResCamY, sizeResCamX, sizeResCamY);
            break;
        }
        default:{
            bg.draw();
            break;
        }
    }

    if (drawFps) {
        ofSetColor(255,255,255);
        font.drawString(ofToString(ofGetFrameRate()), 200, 50);
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
    
#ifdef TARGET_RASPBERRY_PI
    
    string state_buttonL;
    buttonL.getval_gpio(state_buttonL);
    string state_buttonR;
    buttonR.getval_gpio(state_buttonR);
    //ofLog()<< "buttons " << state_buttonL << " / " << state_buttonR;
    //ofLog()<< "buttons int: " << ofFromString<bool>(state_buttonL) << " / " << ofFromString<bool>(state_buttonR);
    
    if (ofFromString<bool>(state_buttonL) && buttonLreleased) {
        buttonLPressed = 1;
        buttonLreleased = 0;
        //ofLog() << "left button pressed";
    }
    if (ofFromString<bool>(state_buttonR) && buttonRreleased) {
        buttonRPressed = 1;
        buttonRreleased = 0;
        //ofLog() << "right button pressed";
    }
    if (!(ofFromString<bool>(state_buttonL))) {buttonLreleased = 1;}
    if (!(ofFromString<bool>(state_buttonR))) {buttonRreleased = 1;}
    
#endif
    
    
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
    
    tickPar.setName("Ticket");
    //
    tickPar.add(print.set("print", 0));
    tickPar.add(ticketMarginXLeft.set("Marge Gauche", 10, 0, fboMaxSizeX/10));
    tickPar.add(ticketMarginXRight.set("Marge Droite", 10, 0, fboMaxSizeX/10));
    tickPar.add(ticketMarginYTop.set("Marge Haut", 10, 0, fboMaxSizeY/5));
    tickPar.add(ticketMarginYBottom.set("Marge Bas", 100, 0, fboMaxSizeY/5));
    tickPar.add(sizeTktX.set("Largeur", 635, 0, fboMaxSizeX));
    tickPar.add(sizeTktY.set("Hauteur", 635, 0, fboMaxSizeY));
    tickPar.add(textMargin.set("Marge Y texte", 20, 0, fboMaxSizeY/5));
    tickPar.add(fontName.set("Nom police", OF_TTF_SANS));
    tickPar.add(textFontSize.set("Taille police", 16, 0, 80));
    //
    parameters.add(tickPar);
    
    coords.setName("Coordonnees displays");
    // coordonnées du cadre principal
    main.setName("Principal");
    main.add(posMainCamX.set("position X", 493, 0, ofGetWidth()));
    main.add(posMainCamY.set("position Y", 73, 0, ofGetHeight()));
    main.add(sizeMainCamX.set("taille X", 934, 0, ofGetWidth()));
    main.add(sizeMainCamY.set("taille Y", 934, 0, ofGetHeight()));
    
    // coordonnées du cadre choix gauche
    choiceL.setName("Choix gauche");
    choiceL.add(posLCamX.set("position X", 1500, 0, ofGetWidth()));
    choiceL.add(posLCamY.set("position Y", 677, 0, ofGetHeight()));
    choiceL.add(sizeLCamX.set("taille X", 330, 0, ofGetWidth()));
    choiceL.add(sizeLCamY.set("taille Y", 330, 0, ofGetHeight()));
    // coordonnées du cadre choix droit
    choiceR.setName("Choix droit");
    choiceR.add(posRCamX.set("position X", 1500, 0, ofGetWidth()));
    choiceR.add(posRCamY.set("position Y", 677, 0, ofGetHeight()));
    choiceR.add(sizeRCamX.set("taille X", 330, 0, ofGetWidth()));
    choiceR.add(sizeRCamY.set("taille Y", 330, 0, ofGetHeight()));
    
    // coordonnées du cadre résultat
    res.setName("Resultat");
    res.add(posResCamX.set("position X", 1500, 0, ofGetWidth()));
    res.add(posResCamY.set("position Y", 677, 0, ofGetHeight()));
    res.add(sizeResCamX.set("taille X", 330, 0, ofGetWidth()));
    res.add(sizeResCamY.set("taille Y", 330, 0, ofGetHeight()));
    //
    coords.add(main);
    coords.add(choiceL);
    coords.add(choiceR);
    coords.add(res);
    parameters.add(coords);
    
    cams.setup_GUI();
    //
    parameters.add(cams.cameras);
    
    leds.setup_GUI();
    //
    parameters.add(leds.LEDs);
    
    timers.setName("Timers");
    //
    timers.add(frameRate.set("FPS",  30, 0, 150));
    timers.add(mainTimer.set("Defaut",  4, 0, maxTimer));
    timers.add(compileTimer.set("Compilation",  4, 0, maxTimer));
    timers.add(profileTimer.set("profil",  4, 0, maxTimer));
    timers.add(flashTimer.set("flash",  4, 0, maxTimer));
    timers.add(countDownTimer.set("countdown",  0.5, 0, maxTimer));
    timers.add(printingTimer.set("Print",  4, 0, maxTimer));
    timers.add(questionTimer.set("question",  5, 0, maxTimer));
    timers.add(drawFps.set("Draw FPS", 0));
    //timers.add(antibounceTimer.set("Anti-rebonds",  30, 0, maxTimer));
    //
    parameters.add(timers);
    
    gui.setup(parameters);
    
    gui.getGroup("Coordonnees displays").minimize();
    gui.getGroup("Fichiers").getGroup("Questions").minimize();
    
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
    
    /// TODO: use latest CSV version
    
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
string ofApp::tabText(string& s, int offset){
    string result = s;
    int spaces = offset-s.size();
    for (size_t i = 0; i< spaces; ++i) {
        result+=' ';
    }
    result+= " : ";
    return result;
}
