//
//  ofxCrvsWindow.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsBox.hpp"

#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

void Box::apply(glm::vec3 &v) const {
  // Scale
  v.x *= getWidth();
  v.y *= getHeight();
  v.z *= getDepth();

  // Apply transformation
  v = glm::vec3(getLocalTransformMatrix() * glm::vec4(v, 1.0));
}

} // namespace ofxCrvs
