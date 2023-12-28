//
//  ofxCrvsMsh.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#include "ofxCrvsMsh.h"

namespace ofxCrvs {

float Msh::componentAt(Component c, float pos) const {
  pos = calcPos(pos);
  float value;
  if (c == Component::X) {
    glm::vec3 xV = this->xCrv->uVector(pos, true);
    value = xV.y;
  } else if (c == Component::Y) {
    glm::vec3 yV = this->yCrv->uVector(pos, true);
    value = quantize(yV.y);
  } else {
    glm::vec3 zV = this->zCrv->uVector(pos, true);
    value = quantize(zV.y);
  }
  value = bipolarize(value);
  value = ampBias(value, pos);
  return value;
}

}  // namespace ofxCrvs
