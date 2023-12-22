//
//  ofxCrvsLsjs.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#include "ofxCrvsLsjs.hpp"

namespace ofxCrvs {

float Lsjs::componentAt(Component c, float pos)
{
    pos = calcPos(pos);
    float value;
    if (c == Component::X) {
        glm::vec2 xV = this->xCrv->uVector(pos, true);
        value = xV.y;
    } else {
        glm::vec2 yV = this->yCrv->uVector(pos, true);
        value = quantize(yV.y);
    }
    value = bipolarize(value);
    value = ampBias(value, pos);
    return value;
}

} // namespace ofxCrvs
