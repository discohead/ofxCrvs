//
//  ofxCrvsLsjs.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#pragma once

#ifndef ofxCrvsLsjs_hpp
#define ofxCrvsLsjs_hpp

#include <functional>

#include "ofMain.h"
#include "ofxCrvsCrv.h"

namespace ofxCrvs {

using VctrOp = std::function<glm::vec2(glm::vec2)>;

class Lsjs : public Crv {
public:
  std::shared_ptr<Crv> xCrv;
  std::shared_ptr<Crv> yCrv;

  Lsjs() : Crv() {}
  Lsjs(std::shared_ptr<Crv> xCrv, std::shared_ptr<Crv> yCrv)
      : Crv(), xCrv(xCrv), yCrv(yCrv){};
  Lsjs(Window window, std::shared_ptr<Crv> xCrv, std::shared_ptr<Crv> yCrv)
      : Crv(window), xCrv(xCrv), yCrv(yCrv){};

  float componentAt(Component c, float pos);
};

} // namespace ofxCrvs

#endif /* ofxCrvsLsjs_hpp */
