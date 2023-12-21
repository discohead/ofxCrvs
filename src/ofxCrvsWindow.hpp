//
//  ofxCrvsWindow.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#pragma once

#ifndef ofxCrvsWindow_hpp
#define ofxCrvsWindow_hpp

#include "ofMain.h"

namespace ofxCrvs {

class Window {
public:
    glm::vec2 origin;
    int width;
    int height;
    glm::vec2 translation;
    glm::vec2 scale;
    float rotation;
    
    Window(int width, int height) : Window(glm::vec2(0.f), width, height, glm::vec2(0.f), glm::vec2(1.f), 0.f) {}
    Window(int x, int y, int width, int height) : Window(glm::vec2(x, y), width, height, glm::vec2(0.f), glm::vec2(1.f), 0.f) {}
    Window(glm::vec2 origin, int width, int height, glm::vec2 translation, glm::vec2 scale, float rotation) : origin(origin), width(width), height(height), translation(translation), scale(scale), rotation(rotation) {};
    
    float getWidth();
    float getHeight();
    glm::vec2 getCenter();
    glm::vec2 apply(glm::vec2 v);
    
private:
    glm::vec2 transform(glm::vec2 v);
};

} // namespace ofxCrvs
#endif /* ofxCrvsWindow_hpp */
