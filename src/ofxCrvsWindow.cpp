//
//  ofxCrvsWindow.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsWindow.hpp"
#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

float Window::getWidth() {
    return width * scale.x;
}

float Window::getHeight() {
    return height * scale.y;
}

glm::vec2 Window::getCenter() {
    return glm::vec2(origin.x + width / 2.f, origin.y + height / 2.f);
}

glm::vec2 Window::apply(glm::vec2 v) {
    v.x *= (getWidth() - 1);
    v.y *= (getHeight() - 1);
    v = transform(v);
    v += origin;
    return Utils::clipped(v, origin.x, origin.x + getWidth(), origin.y, origin.y + getHeight());
}

glm::vec2 Window::transform(glm::vec2 v) {
    return Utils::transform(v, getCenter(), scale, translation, rotation);
}

} // namespace ofxCrvs
