//
// Created by Jared McFarland on 12/30/23.
//
#pragma once

#ifndef OFXCRVSHYPR_H
#define OFXCRVSHYPR_H

#include <utility>

#include "ofMain.h"
#include "ofxCrvsCrv.h"

namespace ofxCrvs {
class Hypr : public Crv {

public:
  std::shared_ptr<Crv> xCrv;
  std::shared_ptr<Crv> yCrv;
  std::shared_ptr<Crv> zCrv;
  std::shared_ptr<Crv> wCrv;

  using Crv::Crv;

  static std::shared_ptr<Hypr> create(const std::shared_ptr<Crv> &xCrv,
                                      const std::shared_ptr<Crv> &yCrv,
                                      const std::shared_ptr<Crv> &zCrv,
                                      const std::shared_ptr<Crv> &wCrv) {
    return std::make_shared<Hypr>(xCrv, yCrv, zCrv, wCrv);
  }

  Hypr(std::shared_ptr<Crv> xCrv, std::shared_ptr<Crv> yCrv,
       std::shared_ptr<Crv> zCrv, std::shared_ptr<Crv> wCrv)
      : Crv(), xCrv(std::move(xCrv)), yCrv(std::move(yCrv)),
        zCrv(std::move(zCrv)), wCrv(std::move(wCrv)){};

  Hypr(Box box, std::shared_ptr<Crv> xCrv, std::shared_ptr<Crv> yCrv,
       std::shared_ptr<Crv> zCrv, std::shared_ptr<Crv> wCrv)
      : Crv(std::move(box)), xCrv(std::move(xCrv)), yCrv(std::move(yCrv)),
        zCrv(std::move(zCrv)), wCrv(std::move(wCrv)){};

  float componentAt(Component c, float pos) const override;
  glm::vec4 uVector4(float pos, bool transformed) const;
  glm::vec4 wVector4(float pos, bool transformed) const;
  std::array<float, 4> uFloat4(float pos, bool transformed) const;
  std::array<float, 4> wFloat4(float pos, bool transformed) const;
};
} // namespace ofxCrvs

#endif // OFXCRVSHYPR_H
