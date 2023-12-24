//
//  ofxCrvsUtils.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include "ofxCrvsUtils.hpp"

#include <algorithm>

namespace ofxCrvs {

glm::vec3 Utils::transform(glm::vec3 vector, glm::vec3 center, glm::vec3 scale,
                           glm::vec3 translation, float rotationDegrees) {
  if (rotationDegrees != 0.f) {
    vector -= center;
    vector = rotateVector(vector, rotationDegrees);
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
  return vector;
}

std::vector<glm::vec3> Utils::transform(std::vector<glm::vec3> vector,
                                        glm::vec3 center, glm::vec3 scale,
                                        glm::vec3 translation,
                                        float rotationDegrees) {
  std::vector<glm::vec3> transformed;
  for (auto& v : vector) {
    glm::vec3 tV = transform(v, center, scale, translation, rotationDegrees);
    transformed.push_back(tV);
  }
  return transformed;
}

// Utility function to rotate a vector by a given angle around its perpendicular
// axis
glm::vec3 Utils::rotateVector(const glm::vec3& vec, float angleDegrees) {
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
  return glm::rotate(vec, angleRadians, rotationAxis);
}

glm::vec3 Utils::clipped(const glm::vec3& point, const ofBoxPrimitive& box) {
  glm::vec3 boxCenter = box.getPosition();
  float halfWidth = box.getWidth() * 0.5f;
  float halfHeight = box.getHeight() * 0.5f;
  float halfDepth = box.getDepth() * 0.5f;

  glm::vec3 minBound = boxCenter - glm::vec3(halfWidth, halfHeight, halfDepth);
  glm::vec3 maxBound = boxCenter + glm::vec3(halfWidth, halfHeight, halfDepth);

  glm::vec3 clippedPoint;
  clippedPoint.x = ofClamp(point.x, minBound.x, maxBound.x);
  clippedPoint.y = ofClamp(point.y, minBound.y, maxBound.y);
  clippedPoint.z = ofClamp(point.z, minBound.z, maxBound.z);

  return clippedPoint;
}

}  // namespace ofxCrvs
