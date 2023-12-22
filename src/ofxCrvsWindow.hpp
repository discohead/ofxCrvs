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

class Window : public ofRectangle {
public:
    using ofRectangle::ofRectangle;
    
    glm::vec2 apply(glm::vec2 v);
    
};

} // namespace ofxCrvs
#endif /* ofxCrvsWindow_hpp */
