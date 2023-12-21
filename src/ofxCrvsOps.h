
#pragma once

#include <functional>
#include "ofMain.h"

namespace ofxCrvs {

using FloatOp = std::function<float(float)>;

class Ops {
public:
    float pos2Rad(float pos);
    FloatOp bipolarize(FloatOp unipolarOp);
    FloatOp rectify(FloatOp bipolarOp);
    FloatOp c(float value);
    FloatOp phasor();
    FloatOp saw();
    FloatOp tri(FloatOp s);
    FloatOp tri();
    FloatOp tri(float s);
    FloatOp sine(FloatOp fb);
    FloatOp sine();
    FloatOp sine(float fb);
    
private:
    ofBaseApp* app = ofGetAppPtr();
};
} // namespace ofxCrvs
