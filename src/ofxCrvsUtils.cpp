//
//  ofxCrvsUtils.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/21/23.
//

#include <algorithm>

#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

glm::vec2 Utils::transform(glm::vec2 vector, glm::vec2 center, glm::vec2 scale, glm::vec2 translation, float rotationDegrees)
{
    if (rotationDegrees != 0.f) {
        vector -= center;
        vector = glm::rotate(vector, glm::radians(rotationDegrees));
        vector += center;
    }
    
    if (scale != glm::vec2(1.f)) {
        vector.x *= scale.x;
        vector.y *= scale.y;
    }
    
    if (translation != glm::vec2(0.f)) {
        vector += translation;
    }
    return vector;
}

std::vector<glm::vec2> Utils::transform(std::vector<glm::vec2> vector, glm::vec2 center, glm::vec2 scale, glm::vec2 translation, float rotationDegrees)
{
    std::vector<glm::vec2> transformed;
    for (auto& v : vector) {
        glm::vec2 tV = transform(v, center, scale, translation, rotationDegrees);
        transformed.push_back(tV);
    }
    return transformed;
}

glm::vec2 Utils::clipped(glm::vec2 vector, float xMin, float xMax, float yMin, float yMax)
{
    vector.x = std::max(xMin, std::min(xMax, vector.x));
    vector.y = std::max(yMin, std::min(yMax, vector.y));
    return vector;
}

} // namespace ofxCrvs
