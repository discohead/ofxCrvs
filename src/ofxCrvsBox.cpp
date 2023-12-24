//
//  ofxCrvsWindow.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsBox.hpp"

#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

void Box::apply(glm::vec3& v) const {
  float w = getWidth();
  float h = getHeight();
  float d = getDepth();

  // Scale
  v.x *= w;
  v.y *= h;
  v.z *= d;

  // Translate to center
  v.x -= w * 0.5f;
  v.y -= h * 0.5f;
  v.z -= d * 0.5f;

  // Apply transformation
  v = glm::vec3(getLocalTransformMatrix() * glm::vec4(v, 1.0));
}

}  // namespace ofxCrvs
