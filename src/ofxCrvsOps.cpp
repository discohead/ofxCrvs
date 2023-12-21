
#include "ofxCrvsOps.h"

namespace ofxCrvs {

float Ops::pos2Rad(float pos)
{
    pos = ofClamp(pos, 0.f, 1.f);
    float degrees = pos * 360.f;
    return ofDegToRad(degrees);
    return pos;
}

FloatOp Ops::bipolarize(FloatOp unipolarOp)
{
    return [unipolarOp](float pos) {
        return 2.f * unipolarOp(pos) - 1.f;
    };
}

FloatOp Ops::rectify(FloatOp bipolarOp)
{
    return [bipolarOp](float pos) {
        float bipolarValue = bipolarOp(pos);
        return bipolarValue * 0.5f + 0.5f;
    };
}

FloatOp Ops::c(float value)
{
    return [value](float /*pos*/) {
        return value;
    };
}

FloatOp Ops::phasor()
{
    return [](float pos) {
        return pos;
    };
}

FloatOp Ops::saw()
{
    return [](float pos) {
        return 1.f - pos;
    };
}

FloatOp Ops::tri(FloatOp s)
{
    return [s](float pos) {
        float sValue = 0.5f;
        if (s) sValue = s(pos);
        return pos < sValue ? pos / sValue : 1.f - ((pos - sValue) / (1.f - sValue));
    };
}

FloatOp Ops::tri()
{
    return tri(nullptr);
}

FloatOp Ops::tri(float s)
{
    return tri(c(s));
}

FloatOp Ops::sine(FloatOp fb)
{
    return [fb, this](float pos) {
        if (fb) {
            float fbScale = fb(pos);
            pos = pos + fbScale * (std::sin(pos2Rad(pos)) * 0.5f) + 0.5f;
        }
        return (std::sin(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
    };
}

FloatOp Ops::sine()
{
    return sine(nullptr);
}

FloatOp Ops::sine(float fb)
{
    return sine(c(fb));
}

} // namespace Ops
