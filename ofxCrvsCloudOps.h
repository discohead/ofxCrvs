#pragma once

#include "ofMain.h"
#include <functional>

namespace ofxCrvs {

using CloudOp = function<vector<glm::vec3>(vector<glm::vec3>)>;

class CloudOps {
public:

  CloudOp zero() {
    return [](vector<glm::vec3> cloud) {
      return vector<glm::vec3>(cloud.size(), glm::vec3(0.0f));
    };
  };

  CloudOp tripleFloatOp(FloatOp xOp, FloatOp yOp, FloatOp zOp) {
    return [xOp, yOp, zOp](vector<glm::vec3> cloud) {
      vector<glm::vec3> newCloud;
      for (int i = 0; i < cloud.size(); i++) {
        newCloud.push_back(glm::vec3(xOp(cloud[i].x), yOp(cloud[i].y), zOp(cloud[i].z)));
      }
      return newCloud;
    };
  };

};

} // namespace ofxCrvs