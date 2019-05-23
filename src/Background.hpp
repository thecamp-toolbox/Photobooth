//
//  Background.hpp
//  Photobooth
//
//  Created by Pascal Baltazar on 21/05/2019.
//

#ifndef Background_hpp
#define Background_hpp

#include "ofMain.h"

class Background {
public:
    
    void load(string path);
    void next();
    void draw();
    
    
private:
    ofTexture buffer[2];
    bool textureToken{0};
    
};

#endif /* Background_hpp */
