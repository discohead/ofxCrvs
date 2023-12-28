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

using VctrOp = std::function<glm::vec3(glm::vec3)>;

class Msh : public Crv {
 public:
  std::shared_ptr<Crv> xCrv;
  std::shared_ptr<Crv> yCrv;
  std::shared_ptr<Crv> zCrv;

  using Crv::Crv;

  Msh(std::shared_ptr<Crv> xCrv, std::shared_ptr<Crv> yCrv,
      std::shared_ptr<Crv> zCrv)
      : Crv(), xCrv(xCrv), yCrv(yCrv), zCrv(zCrv){};
  Msh(Box box, std::shared_ptr<Crv> xCrv, std::shared_ptr<Crv> yCrv,
      std::shared_ptr<Crv> zCrv)
      : Crv(box), xCrv(xCrv), yCrv(yCrv), zCrv(zCrv){};

  float componentAt(Component c, float pos) const;
};

}  // namespace ofxCrvs

#endif /* ofxCrvsLsjs_hpp */
