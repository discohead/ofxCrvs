//
//  ofxCrvsWindow.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsBox.hpp"

#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

glm::vec3 Box::apply(glm::vec3 v) {
  v.x *= (getWidth() - 1);
  v.y *= (getHeight() - 1);
  v.z *= (getDepth() - 1);
  glm::vec3 origin = getPosition();
  v += origin;
  return Utils::clipped(v, *this);
}

}  // namespace ofxCrvs
