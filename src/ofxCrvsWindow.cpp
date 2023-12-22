//
//  ofxCrvsWindow.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsWindow.hpp"
#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

glm::vec2 Window::apply(glm::vec2 v) {
    v.x *= (getWidth() - 1);
    v.y *= (getHeight() - 1);
    glm::vec2 origin = getTopLeft();
    v += origin;
    return Utils::clipped(v, getMinX(), getMaxX(), getMinY(), getMaxY());
}

} // namespace ofxCrvs
