//
//  ofxCrvsWindow.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#pragma once

#ifndef ofxCrvsWindow_hpp
#define ofxCrvsWindow_hpp

#include "ofMain.h"

namespace ofxCrvs {

class Box : public ofBoxPrimitive {
 public:
  using ofBoxPrimitive::ofBoxPrimitive;

  void apply(glm::vec3& v) const;
};

}  // namespace ofxCrvs
#endif /* ofxCrvsWindow_hpp */
