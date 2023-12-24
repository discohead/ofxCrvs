//
//  ofxCrvsEdg.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#pragma once

#ifndef ofxCrvsEdg_hpp
#define ofxCrvsEdg_hpp

#include "ofMain.h"

namespace ofxCrvs {

class Crv;

class Edg {
 public:
  Edg(glm::vec3 source, glm::vec3 target, int resolution)
      : source(source),
        target(target),
        resolution(resolution),
        translation(glm::vec3(0)),
        scale(glm::vec3(1)),
        rotation(0){};

  Edg(glm::vec3 source, glm::vec3 target, int resolution, glm::vec3 translation,
      glm::vec3 scale, float rotation)
      : source(source),
        target(target),
        resolution(resolution),
        translation(translation),
        scale(scale),
        rotation(rotation){};

  glm::vec3 source;
  glm::vec3 target;

  int resolution;
  glm::vec3 translation;
  glm::vec3 scale;

  float rotation;

  float length() const;
  glm::vec3 at(float pos) const;
  glm::vec3 midpoint() const;
  void transformed();
  std::vector<glm::vec3> points() const;
  std::vector<glm::vec3> points(int numPoints) const;
  float angle() const;
  glm::vec3 getPerpendicularPoint(glm::vec3 point, float magnitude) const;
  std::vector<glm::vec3> getCrvPoints(Crv crv, int resolution) const;
  std::vector<glm::vec3> getCrvPoints(Crv crv) const;
  glm::vec3 asVector() const;
};

}  // namespace ofxCrvs

#endif /* ofxCrvsEdg_hpp */
