//
//  ofxCrvsUtils.hpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#pragma  once

#ifndef ofxCrvsUtils_hpp
#define ofxCrvsUtils_hpp

#include "ofMain.h"

namespace ofxCrvs {

class Utils {
public:
    static glm::vec2 transform(glm::vec2 vector, glm::vec2 center, glm::vec2 scale, glm::vec2 translation, float rotationDegrees);
    static std::vector<glm::vec2> transform(std::vector<glm::vec2> vector, glm::vec2 center, glm::vec2 scale, glm::vec2 translation, float rotationDegrees);
    static glm::vec2 clipped(glm::vec2 vector, float xMin, float xMax, float yMin, float yMax);
};

} // namespace ofxCrvs

#endif /* ofxCrvsUtils_hpp */
