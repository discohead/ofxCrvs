//
//  ofxCrvsEdg.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#include "ofxCrvsEdg.hpp"

#include "ofxCrvsCrv.h"
#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

float Edg::length() const { return glm::distance(source, target); }

glm::vec3 Edg::at(float pos) const {
  glm::vec3 point;
  point.x = ofMap(pos, 0, 1, source.x, target.x);
  point.y = ofMap(pos, 0, 1, source.y, target.y);
  point.z = 0.f;

  return point;
}

glm::vec3 Edg::midpoint() const { return at(0.5); }

void Edg::transformed() {
  Utils::transform(source, midpoint(), scale, translation, rotation,
                   Utils::zAxis);
  Utils::transform(target, midpoint(), scale, translation, rotation,
                   Utils::zAxis);
}

std::vector<glm::vec3> Edg::points() const { return points(resolution); }

std::vector<glm::vec3> Edg::points(int numPoints) const {
  std::vector<glm::vec3> points;
  for (int i = 0; i < numPoints; ++i) {
    points.push_back(at((float)i / (float)numPoints));
  }
  return points;
}

float Edg::angle() const {
  return glm::atan(target.y - source.y, target.x - source.x);
}

glm::vec3 Edg::getPerpendicularPoint(glm::vec3 point, float magnitude) const {
  glm::vec3 dir = this->asVector();
  dir = glm::normalize(dir);

  // Find an arbitrary axis that is perpendicular to 'dir'
  glm::vec3 arbitraryAxis(1.0f, 0.0f, 0.0f);
  if (glm::length(dir - arbitraryAxis) < 0.0001f ||
      glm::length(dir + arbitraryAxis) < 0.0001f) {
    arbitraryAxis =
        glm::vec3(0.0f, 1.0f, 0.0f);  // Choose a different axis if 'dir' is
                                      // parallel to the initial arbitraryAxis
  }
  glm::vec3 rotationAxis = glm::cross(dir, arbitraryAxis);
  rotationAxis = glm::normalize(rotationAxis);

  // Rotate 'dir' 90 degrees around 'rotationAxis'
  dir = glm::rotate(dir, glm::half_pi<float>(), rotationAxis);
  dir *= magnitude;

  return point + dir;
}

std::vector<glm::vec3> Edg::getCrvPoints(Crv crv, int resolution) const {
  std::vector<glm::vec3> edgPoints = points(resolution);
  std::vector<glm::vec3> crvPoints;
  for (int i = 0; i < resolution; ++i) {
    float x = (float)i / (float)(resolution - 1.f);
    float mag = crv.yAt(x);
    crvPoints.push_back(getPerpendicularPoint(edgPoints[i], mag));
  }
  return crvPoints;
}

std::vector<glm::vec3> Edg::getCrvPoints(Crv crv) const {
  return getCrvPoints(crv, resolution);
}

glm::vec3 Edg::asVector() const { return target - source; }

}  // namespace ofxCrvs
