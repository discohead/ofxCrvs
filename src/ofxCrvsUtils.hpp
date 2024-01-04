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
#include "ofxCrvsConstants.h"

namespace ofxCrvs {

class Utils {
public:
  static void
  transform(glm::vec3 &vector, const glm::vec3 &center, const glm::vec3 &scale,
            const glm::vec3 &translation, float rotationDegrees,
            const glm::vec3 &rotationAxis = glm::vec3(0.f, 0.f, 1.f));

  static void transform(std::vector<glm::vec3> vectors, const glm::vec3 &center,
                        const glm::vec3 &scale, const glm::vec3 &translation,
                        float rotationDegrees,
                        const glm::vec3 &rotationAxis = glm::vec3(0.f, 0.f,
                                                                  1.f));

  static void clipped(glm::vec3 &point, const ofBoxPrimitive &box);

  static void rotateVector(glm::vec3 &vec, float angleDegrees);
};

} // namespace ofxCrvs

#endif /* ofxCrvsUtils_hpp */
