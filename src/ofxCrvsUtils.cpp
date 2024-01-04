//
//  ofxCrvsUtils.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsUtils.hpp"

#include <algorithm>

namespace ofxCrvs {

void Utils::transform(glm::vec3& vector, const glm::vec3& center,
                      const glm::vec3& scale, const glm::vec3& translation,
                      float rotationDegrees, const glm::vec3& rotationAxis) {
  if (rotationDegrees != 0.f) {
    vector -= center;
    vector = glm::rotate(vector, glm::radians(rotationDegrees), rotationAxis);
    vector += center;
  }

  if (scale != glm::vec3(1.f)) {
    vector.x *= scale.x;
    vector.y *= scale.y;
    vector.z *= scale.z;
  }

  if (translation != glm::vec3(0.f)) {
    vector += translation;
  }
}

void Utils::transform(std::vector<glm::vec3> vectors, const glm::vec3& center,
                      const glm::vec3& scale, const glm::vec3& translation,
                      float rotationDegrees, const glm::vec3& rotationAxis) {
  for (auto& v : vectors) {
    transform(v, center, scale, translation, rotationDegrees, rotationAxis);
  }
}

// Utility function to rotate a vector by a given angle around its perpendicular
// axis
void Utils::rotateVector(glm::vec3& vec, float angleDegrees) {
  // Convert angle to radians
  float angleRadians = glm::radians(angleDegrees);

  // Find an arbitrary axis that is not parallel or anti-parallel to 'vec'
  glm::vec3 arbitraryAxis(1.0f, 0.0f, 0.0f);
  if (glm::length(vec - arbitraryAxis) < 0.0001f ||
      glm::length(vec + arbitraryAxis) < 0.0001f) {
    arbitraryAxis = glm::vec3(
        0.0f, 1.0f, 0.0f);  // Choose a different axis if 'vec' is parallel or
                            // anti-parallel to the initial arbitraryAxis
  }

  // Find a perpendicular axis using cross product
  glm::vec3 rotationAxis = glm::cross(vec, arbitraryAxis);
  rotationAxis = glm::normalize(rotationAxis);

  // Rotate the vector
  vec = glm::rotate(vec, angleRadians, rotationAxis);
}

void Utils::clipped(glm::vec3& point, const ofBoxPrimitive& box) {
  glm::vec3 boxCenter = box.getPosition();
  float halfWidth = box.getWidth() * 0.5f;
  float halfHeight = box.getHeight() * 0.5f;
  float halfDepth = box.getDepth() * 0.5f;

  glm::vec3 minBound = boxCenter - glm::vec3(halfWidth, halfHeight, halfDepth);
  glm::vec3 maxBound = boxCenter + glm::vec3(halfWidth, halfHeight, halfDepth);

  point.x = ofClamp(point.x, minBound.x, maxBound.x);
  point.y = ofClamp(point.y, minBound.y, maxBound.y);
  point.z = ofClamp(point.z, minBound.z, maxBound.z);
}

}  // namespace ofxCrvs
