//
// Created by Jared McFarland on 12/30/23.
//

#include "ofxCrvsHypr.h"

namespace ofxCrvs {

float Hypr::componentAt(Component c, float pos) const {
  pos = calcPos(pos);
  float value;
  if (c == Component::X) {
    glm::vec3 xV = this->xCrv->uVector(pos, true);
    value = xV.y;
  } else if (c == Component::Y) {
    glm::vec3 yV = this->yCrv->uVector(pos, true);
    value = quantize(yV.y);
  } else if (c == Component::Z) {
    glm::vec3 zV = this->zCrv->uVector(pos, true);
    value = quantize(zV.y);
  } else {
    glm::vec3 wV = this->wCrv->uVector(pos, true);
    value = quantize(wV.y);
  }
  value = bipolarize(value);
  value = ampBias(value, pos);
  return value;
}

glm::vec4 Hypr::uVector4(float pos, bool transformed) const {
  glm::vec3 v = Crv::uVector(pos, transformed);
  return {v, wAt(pos)};
}

glm::vec4 Hypr::wVector4(float pos, bool transformed) const {
  glm::vec3 v = Crv::wVector(pos, transformed);
  boxed(v);
  return {v, wAt(pos)};
}

std::array<float, 4> Hypr::uFloat4(float pos, bool transformed) const {
  glm::vec4 v = uVector4(pos, transformed);
  return {v.x, v.y, v.z, v.w};
}

std::array<float, 4> Hypr::wFloat4(float pos, bool transformed) const {
  glm::vec4 v = wVector4(pos, transformed);
  return {v.x, v.y, v.z, v.w};
}

} // namespace ofxCrvs