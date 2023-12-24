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

  float length();
  glm::vec3 at(float pos);
  glm::vec3 midpoint();
  Edg transformed();
  std::vector<glm::vec3> points();
  std::vector<glm::vec3> points(int numPoints);
  float angle();
  glm::vec3 getPerpendicularPoint(glm::vec3 point, float magnitude);
  std::vector<glm::vec3> getCrvPoints(Crv crv, int resolution);
  std::vector<glm::vec3> getCrvPoints(Crv crv);
  glm::vec3 asVector();
};

}  // namespace ofxCrvs

#endif /* ofxCrvsEdg_hpp */
