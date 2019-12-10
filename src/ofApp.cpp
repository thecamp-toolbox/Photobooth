#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(10);

    setupGUI();

    cams.setup();
    leds.setup();

    //___________________________
    // Loading Images:

    for (size_t j = 0; j < nCountdown; ++j){
        ofLoadImage(countdowns[j], ("/data/BG-simple/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png"));
        ofLog() << ("/data/BG-simple/"+backgroundFiles[COUNTDOWN]+to_string(j+1)+".png");
    }
    for (size_t j = 0; j < nTimer; ++j){
        ofLoadImage(timerV[j], ("/data/BG-simple/timer"+to_string(j+1)+".png"));
        ofLog() << ("/data/BG-simple/timer"+to_string(j+1)+".png");
    }
    ofLoadImage(profilTicketThecamp,   ("/data/logo_thecamp.png"));
    ofLoadImage(profilTicketClient,   ("/data/logo_client.png"));

    //bg.load("/data/BG-simple/"+backgroundFiles[INIT]);

    ofEnableAlphaBlending();

#ifdef TARGET_RASPBERRY_PI

    buttonL.setup("17");
    buttonL.export_gpio();
    buttonL.setdir_gpio("in");

    buttonR.setup("27");
    buttonR.export_gpio();
    buttonR.setdir_gpio("in");

    ledL.setup("13");
    ledL.export_gpio();
    ledL.setdir_gpio("out");

    ledR.setup("19");
    ledR.export_gpio();
    ledR.setdir_gpio("out");

    flashRelay.setup("26");
    flashRelay.export_gpio();
    flashRelay.setdir_gpio("out");


#endif

}

//--------------------------------------------------------------
void ofApp::update(){

    if (GUIhide) PBtimer++;
    getButtons();
    switch (currentState) {
        case INIT: {
            PBtimer++;
            if (PBtimer%5==0)
                cams.update_all();
            if (PBtimer==1) {
                bg.next();
                GUIhide = 0;
                ofShowCursor();
                bg.load("/data/BG-simple/"+backgroundFiles[STANDBY]);
                ofLoadImage(frame,"/data/BG-simple/"+backgroundFiles[FRAME]+".png");
                leds.currentAnimation = leds.INIT;
                font.load( OF_TTF_SANS,30,true,true);
                ledButtons(1, 1);
            }

                currentState = STANDBY;
                PBtimer = 0;
                loadCSV();
                ofLog() << "CSV loaded";
                ofSerialize(settings,parameters);
                settings.save("/data/settings-simple.xml");
                GUIhide = 1;
                ofHideCursor();
                widthLogoClient = profilTicketClient.getWidth();
                heightLogoClient = profilTicketClient.getHeight();
                ticketWidth = ticketMarginXLeft+ticketMarginXRight+sizeTktX;
                ticketHeight= sizeTktY ;
                fbo.allocate(ticketWidth, ticketHeight, GL_RGBA );
                font.load( OF_TTF_SANS,textFontSize,true,true);
                leds.setup();
                //leds.currentAnimation = leds.NONE;
                ofSetFrameRate(frameRate);

                // setting time offset
                Poco::Timestamp now;
                Poco::LocalDateTime nowLocal(now);

                //timeOffset =  setTime.timestamp() - nowLocal.timestamp();

            break;
        }
        case STANDBY: {
            if (PBtimer==1) {
                leds.currentAnimation = leds.INIT;
                bg.next();
                ofLog() << "STANDBY";
                bg.load("/data/BG-simple/"+backgroundFiles[FRAME]);
                ledButtons(1, 1);
            }

            break;
        }
        case FRAME: {
            ledButtons(1, 1);
            cams.update_one();

            if (PBtimer==1) ofLog() << "FRAME";

            break;
        }
        case COUNTDOWN: {

            cams.update_one();

            if (PBtimer==1) {
                ofLog() << "COUNTDOWN";
                ledButtons(0, 0);
            }

            break;
        }
        case FLASH: {

            cams.update_one();

            if (PBtimer==1) {
                ofLog() << "FLASH";
            }

            break;
        }
        case RESULT: {

            if (PBtimer==1) {
                leds.currentAnimation = leds.NONE;
                ledButtons(1, 1);
                ofLog() << "RESULT, #" << resultCount ;

            }

            break;
        }
        case PRINTING: {
            if (PBtimer==1) {
                ledButtons(0, 0);
                ofLog() << "PRINTING";
                bg.load("/data/BG-simple/"+backgroundFiles[BYE]);

                Poco::Timestamp now;
                Poco::LocalDateTime nowLocal(now);
                Poco::Timestamp saveTime;

                string readableDate = ofToString(day)+'/'+ofToString(month)+'/'+ofToString(year);

                fbo.begin();

                ofClear(255,255,255, 0);
                ofSetColor(255,255,255, 255);
                ofDrawRectangle(0, 0, ticketWidth, ticketHeight);

                result.draw(ticketMarginXLeft, profilSizeY + ticketMarginYTop, sizeResCamX*1.8, sizeResCamY*1.8);
                profilTicketClient.draw(profilMarginX, profilMarginY, profilSizeX, profilSizeY);
                profilTicketThecamp.draw(positionXLogoTC, positionYLogoTC, 235, 37);

                ofSetHexColor(0x000000);
                fontTicket.load( OF_TTF_SANS, 24, true,true);
                fontTicket.drawString(readableDate, positionXDate, positionYDate);
                fontTicket.drawString(customerText, positionXText, positionYText);

                ofSetColor(255,255,255, 255);
                ticket.grabScreen(0, 0, ticketWidth, ticketHeight);
                ticket.mirror(1,0);

                fbo.end();


                string fileName = eventName;
                fileName+='-'+ofToString(year)+'-'+ofToString(month)+'-'+ofToString(day)
                +'-'+ofToString(hour)+'h'+ofToString(min)+'-'+ofToString(ofGetElapsedTimeMillis())+'-';
                fileName+=".png";
                ofLog() << "Photo saved as: " << fileName;

                //ticket.resize(512, 1486);
                ticket.save(photoPath+fileName);

                if (print){
                    string printCommand = "lp "+photoPath+fileName;
                    ofSystem(printCommand);
                }
            }

            break;
        }
        case BYE: {
            if (PBtimer==1) {
                ledButtons(1, 1);
                ofLog() << "BYE";
                bg.load("/data/BG-simple/"+backgroundFiles[STANDBY]);
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
            ofSetColor(255, 255, 255, 255);
            //bg.draw();
            // gui.draw();
            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);

            const std::string DATE_FORMAT = "%Y %b %f %H:%M";

            timeSet = ofToString(year) + "-" + ofToString(month) + "-" + ofToString(day) + "T" + ofToString(hour) + ":" + ofToString(min) + ":00:00+02:00";

            int tzd = 0;

            Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT,
                                        timeSet, setTime, tzd);

            timeNow = "Date et heure: " + ofxTime::Utils::format(setTime, DATE_FORMAT);

//            ofSetColor(250, 250, 0, 250);
//            font.drawString("appuyer sur un des boutons pour lancer l'application" ,350, 280);
//            ofSetColor(255, 255, 255, 255);

            break;
        }
        case STANDBY: {
            ofSetColor(255, 255, 255, 255);
            bg.draw();

            if (buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = FRAME;
                PBtimer = 0;

            }
            break;

        }
        case FRAME: {
            ofSetColor(255, 255, 255, 255);
            cams.draw_one(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0,1920,1080);

            if (PBtimer>mainTimer*ofGetFrameRate() || buttonLPressed || buttonRPressed){
                resetButtons();
                currentState = COUNTDOWN;
                PBtimer = 0;
            }

            break;
        }
        case COUNTDOWN: {
            ofSetColor(255, 255, 255, 255);
            cams.draw_one(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0,1920,1080);
            countdowns[nCountdown-1-currentCountdown].draw(posCDX, posCDY,
                                                           sizeCDX, sizeCDY);

            countdowns[nCountdown-1-currentCountdown].draw(posCDX + 1000, posCDY, sizeCDX, sizeCDY);

            if (PBtimer>countDownTimer*ofGetFrameRate()){
                currentCountdown++;
                resetButtons();
                PBtimer = 0;
                if (currentCountdown==4){
                    leds.currentAnimation = leds.FLASH;
                    flash(0);
                    currentState = FLASH;
                    currentCountdown=0;
                }
            }

            break;
        }
        case FLASH: {
            ofSetColor(255, 255, 255, 255);
            //bg.draw();
            cams.draw_one(posMainCamX, posMainCamY, sizeMainCamX, sizeMainCamY);
            frame.draw(0,0,1920,1080);
            //buffer[textureToken].draw(0,0);

            if (leds.draw) leds.img.draw(leds.X, leds.Y, leds.W, leds.H);

            if (PBtimer>flashTimer*ofGetFrameRate()){
                //if (PBtimer==2){
                result.grabScreen(posResCamX, posResCamY, sizeResCamX, sizeResCamY);
                //}
              //  bg.load("/data/BG-simple/"+backgroundFiles[FRAME]);
              //  bg.next();
                resetButtons();
                currentState = RESULT;
                flash(0);
                PBtimer = 0;
            }

            break;
        }

        case RESULT: {

            ofSetColor(255, 255, 255, 255);
          //  bg.draw();

            result.draw(posResCamX, posResCamY, sizeResCamX, sizeResCamY);
            frame.draw(0,0,1920,1080);
            //drawTimer(mainTimer);
            bg.load("/data/BG-simple/"+backgroundFiles[BYE]);
            bg.next();
            resetButtons();
            currentState = PRINTING;
            PBtimer = 0;
            resultCount = 0;

            break;
        }

        case PRINTING:{
            ofSetColor(255, 255, 255, 255);
            bg.draw();

            if (PBtimer>printingTimer*ofGetFrameRate()){
                bg.next();
                resetButtons();
                currentState = BYE;
                PBtimer = 0;
            }
        }
            break;

        case BYE:{

            ofSetColor(255, 255, 255, 255);
            bg.draw();

            if (PBtimer>mainTimer*ofGetFrameRate() || buttonLPressed || buttonRPressed){
                //bg.next();
                resetButtons();
                currentState = STANDBY;
                PBtimer = 0;
            }
            break;
        }

        default:{
            ofSetColor(255, 255, 255, 255);
            bg.draw();
            break;
        }
    }

    if (drawFps) {
        ofSetColor(127,127,127);
        font.drawString(ofToString(int(ofGetFrameRate())) + " fps", 1700, 80);
        ofSetColor(255, 255, 255, 255);
    }

    if (!GUIhide) {
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofLog() << "QUITTING THE APP at ";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 32) {
        GUIhide = !GUIhide;
        if (GUIhide) {
            ofHideCursor();
            ofSerialize(settings,parameters);
            settings.save("/data/settings-simple.xml");
        }
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
        settings.save("/data/settings-simple.xml");
    }
    if(key=='l'){
        settings.load("/data/settings-simple.xml");
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
void ofApp::drawTimer(int timer){
    int index = PBtimer*11/(timer*frameRate);
    if (index > 10) index = 11;
    timerV[11-index].draw(posTimerX+sizeTimerX, posTimerY,
                       -sizeTimerX, sizeTimerY);
}

//--------------------------------------------------------------
void ofApp::ledButtons(bool L, bool R){
#ifdef TARGET_RASPBERRY_PI
    ledL.setval_gpio(ofToString(L));
    ledR.setval_gpio(ofToString(R));
#endif
}
//--------------------------------------------------------------
void ofApp::flash(bool on){
#ifdef TARGET_RASPBERRY_PI
    flashRelay.setval_gpio(ofToString(on));
#endif
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

    timeStart = ofxTime::Utils::format(nowLocal, Poco::DateTimeFormat::ISO8601_FORMAT);

    ofLog() << "Time NOW !!! : " << timeNow;

    parameters.setName("Reglages");

    event.setName("Evenement");

    event.add(eventName.set("Nom", "VYV-solidarites19"));
    event.add(year.set("Annee", ofFromString<int>(timeStart.substr(0,4)), 2019, 2029));
    event.add(month.set("Mois", ofFromString<int>(timeStart.substr(5,2)), 1, 12));
    event.add(day.set("Jour", ofFromString<int>(timeStart.substr(7,2)), 1, 31));
    event.add(hour.set("Heure", ofFromString<int>(timeStart.substr(9,2)), 0, 23));
    event.add(min.set("Minute", ofFromString<int>(timeStart.substr(11,2)), 0, 59));
    //
    parameters.add(event);

    files.setName("Fichiers");
    //
    files.add(logToFile.set("Log/fichier", 1));
    //
    /*
    questionsFile.setName("Questions");
    questionsFile.add(weightsFilePath.set("Chemin", "questions+.csv"));
    questionsFile.add(weightsCSVcolOffset.set("Col offset", 2, 0, 10));
    //
    files.add(questionsFile);
     */
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
    tickPar.add(profilMarginX.set("Position X logoClient", 20, 0, fboMaxSizeY/5));
    tickPar.add(profilMarginY.set("Position Y logoClient", 20, 0, fboMaxSizeY/5));


    //tickPar.add(textOffsetX.set("Offset X texte", 100, 0, fboMaxSizeX/2));
    tickPar.add(customerText.set("Customer text", "A base camp for exploring the future"));

    tickPar.add(profilSizeY.set("Taille Y profil", 180, 0, 80));
    tickPar.add(profilSizeX.set("Taille X profil", 180, 0, 80));

    tickPar.add(positionYDate.set("Position Y date", 180, 0, 80));
    tickPar.add(positionXDate.set("Position X date", 180, 0, 80));
    tickPar.add(positionYText.set("Position Y text", 180, 0, 80));
    tickPar.add(positionXText.set("Position X text", 180, 0, 80));

    tickPar.add(positionXLogoTC.set("Position X logoTheCamp", 180, 0, 80));
    tickPar.add(positionYLogoTC.set("Position Y logoTheCamp", 180, 0, 80));

    //
    parameters.add(tickPar);

    coords.setName("Coordonnees displays");
    // coordonnées du cadre principal
    main.setName("Principal");
    main.add(posMainCamX.set("position X", 493, 0, ofGetWidth()));
    main.add(posMainCamY.set("position Y", 73, 0, ofGetHeight()));
    main.add(sizeMainCamX.set("taille X", 934, 0, ofGetWidth()));
    main.add(sizeMainCamY.set("taille Y", 934, 0, ofGetHeight()));


    // coordonnées du cadre résultat
    res.setName("Resultat");
    res.add(posResCamX.set("position X", 1500, 0, ofGetWidth()));
    res.add(posResCamY.set("position Y", 677, 0, ofGetHeight()));
    res.add(sizeResCamX.set("taille X", 330, 0, ofGetWidth()));
    res.add(sizeResCamY.set("taille Y", 330, 0, ofGetHeight()));
    //

    // coordonnées du countdown
    countdown.setName("CountDown");
    countdown.add(posCDX.set("position X", 860, 0, ofGetWidth()));
    countdown.add(posCDY.set("position Y", 200, 0, ofGetHeight()));
    countdown.add(sizeCDX.set("taille X", 200, 0, ofGetWidth()));
    countdown.add(sizeCDY.set("taille Y", 200, 0, ofGetHeight()));
    //
    // coordonnées du timer
    timerPos.setName("Timer");
    timerPos.add(posTimerX.set("position X", 860, 0, ofGetWidth()));
    timerPos.add(posTimerY.set("position Y", 200, 0, ofGetHeight()));
    timerPos.add(sizeTimerX.set("taille X", 200, 0, ofGetWidth()));
    timerPos.add(sizeTimerY.set("taille Y", 200, 0, ofGetHeight()));
    //

    coords.add(main);
    coords.add(res);
    coords.add(countdown);
    coords.add(timerPos);
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
    //timers.add(compileTimer.set("Compilation",  4, 0, maxTimer));
    //timers.add(profileTimer.set("profil",  4, 0, maxTimer));
    timers.add(flashTimer.set("flash",  4, 0, maxTimer));
    timers.add(countDownTimer.set("countdown",  0.5, 0, maxTimer));
    timers.add(printingTimer.set("Print",  4, 0, maxTimer));
    //timers.add(questionTimer.set("question",  5, 0, maxTimer));
    timers.add(drawFps.set("Draw FPS", 0));
    //timers.add(antibounceTimer.set("Anti-rebonds",  30, 0, maxTimer));
    //
    parameters.add(timers);

    gui.setup(parameters);

    gui.setSize(300, gui.getHeight());

    gui.getGroup("Evenement").setSize(300, gui.getHeight());
    gui.getGroup("Evenement").setWidthElements(300);
    gui.getGroup("Fichiers").setSize(300, gui.getHeight());
    gui.getGroup("Fichiers").setWidthElements(300);
    gui.getGroup("Ticket").setSize(300, gui.getHeight());
    gui.getGroup("Ticket").setWidthElements(300);
    gui.getGroup("Timers").setSize(300, gui.getHeight());
    gui.getGroup("Timers").setWidthElements(300);
    gui.getGroup("Coordonnees displays").setSize(300, gui.getHeight());
    gui.getGroup("Coordonnees displays").setWidthElements(300);
    gui.getGroup("Cameras").setSize(300, gui.getHeight());
    gui.getGroup("Cameras").setWidthElements(300);
    gui.getGroup("LEDs").setSize(300, gui.getGroup("LEDs").getHeight());
    gui.getGroup("LEDs").setWidthElements(300);

    gui.getGroup("LEDs").minimize();
    gui.getGroup("LEDs").getGroup("visualisation LEDs").minimize();
    gui.getGroup("Coordonnees displays").minimize();
    gui.getGroup("Fichiers").getGroup("Questions").minimize();

    gui.loadFromFile("/data/settings-simple.xml");

    settings.load("/data/settings-simple.xml");
    ofDeserialize(settings, parameters);


    year = ofFromString<int>(timeStart.substr(0,4));
    month = ofFromString<int>(timeStart.substr(5,2));
    day = ofFromString<int>(timeStart.substr(8,2));
    hour = ofFromString<int>(timeStart.substr(11,2));
    min =ofFromString<int>(timeStart.substr(14,2));

    ofLog() << year << " " << month << " " << day << " " << hour << " " << min;

}

//--------------------------------------------------------------
void ofApp::loadCSV(){

    if (logToFile){
        string logPath = eventName;
        ofLogToFile("/data/logs/"+logPath+".txt", true);
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
