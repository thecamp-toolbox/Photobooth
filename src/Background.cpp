//
//  Background.cpp
//  Photobooth
//
//  Created by Pascal Baltazar on 21/05/2019.
//

#include "Background.hpp"

void Background::load(string path){
    ofLoadImage(buffer[!textureToken], path);
}

void Background::next(){
    textureToken=!textureToken;
}

void Background::draw(){
    buffer[textureToken].draw(0,0, 1920, 1080);
}
