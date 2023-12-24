//
//  ofxCrvsUtils.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#pragma once

#ifndef ofxCrvsUtils_hpp
#define ofxCrvsUtils_hpp

#include "ofMain.h"

namespace ofxCrvs {

class Utils {
 public:
  static glm::vec3 transform(glm::vec3 vector, glm::vec3 center,
                             glm::vec3 scale, glm::vec3 translation,
                             float rotationDegrees);
  static std::vector<glm::vec3> transform(std::vector<glm::vec3> vector,
                                          glm::vec3 center, glm::vec3 scale,
                                          glm::vec3 translation,
                                          float rotationDegrees);

  static glm::vec3 clipped(const glm::vec3& point, const ofBoxPrimitive& box);

  static glm::vec3 rotateVector(const glm::vec3& vec, float angleDegrees);
};

}  // namespace ofxCrvs

#endif /* ofxCrvsUtils_hpp */
