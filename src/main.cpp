#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

#ifdef TARGET_RASPBERRY_PI
    ofGLESWindowSettings settings;
    settings.windowMode = OF_FULLSCREEN;
#else
    ofGLFWWindowSettings settings;
    settings.windowMode = OF_FULLSCREEN;
#endif
    auto window = ofCreateWindow(settings);
    auto app = make_shared<ofApp>();
    ofHideCursor();

    ofRunApp(window, app);
    
    return ofRunMainLoop();
}
