//
//  ofxCrvsEdg.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#pragma once

#ifndef ofxCrvsEdg_hpp
#define ofxCrvsEdg_hpp

#include "ofMain.h"

namespace ofxCrvs {

class Crv;

class Edg {
public:
    
    Edg(glm::vec2 source, glm::vec2 target, int resolution) :
        source(source),
        target(target),
        resolution(resolution),
        translation(glm::vec2(0)),
        scale(glm::vec2(1)),
        rotation(0) {};
    
    Edg(glm::vec2 source, glm::vec2 target, int resolution, glm::vec2 translation, glm::vec2 scale, float rotation) :
        source(source),
        target(target),
        resolution(resolution),
        translation(translation),
        scale(scale),
        rotation(rotation) {};
    
    glm::vec2 source;
    glm::vec2 target;
    
    int resolution;
    glm::vec2 translation;
    glm::vec2 scale;
    
    float rotation;
    
    float length();
    glm::vec2 at(float pos);
    glm::vec2 midpoint();
    Edg transformed();
    std::vector<glm::vec2> points();
    std::vector<glm::vec2> points(int numPoints);
    float angle();
    glm::vec2 getPerpendicularPoint(glm::vec2 point, float magnitude);
    std::vector<glm::vec2> getCrvPoints(Crv crv, int resolution);
    std::vector<glm::vec2> getCrvPoints(Crv crv);
    glm::vec2 asVector();
};

} // namespace ofxCrvs

#endif /* ofxCrvsEdg_hpp */
