
#pragma once

#include "ofMain.h"
#include <functional>

namespace ofxCrvs {

using FloatOp = std::function<float(float)>;

class Ops {
public:
  Ops() {
    table.resize(1024);
    for (int i = 0; i < 1024; i++) {
      table[i] = ofRandom(1.0f);
    }
  };
  Ops(vector<float> table) : table(table){};

  FloatOp zero() {
    return [](float) { return 0.0f; };
  };
  FloatOp fourth() {
    return [](float) { return 0.25f; };
  };
  FloatOp third() {
    return [](float) { return 1.f / 3.f; };
  };
  FloatOp half() {
    return [](float) { return 0.5f; };
  };
  FloatOp one() {
    return [](float) { return 1.0f; };
  };
  FloatOp two() {
    return [](float) { return 2.0f; };
  };
  FloatOp three() {
    return [](float) { return 3.0f; };
  };
  FloatOp four() {
    return [](float) { return 4.0f; };
  };
  FloatOp quarterPi() {
    return [](float) { return glm::quarter_pi<float>(); };
  };
  FloatOp thirdPi() {
    return [](float) { return glm::pi<float>() / 3.f; };
  };
  FloatOp halfPi() {
    return [](float) { return glm::half_pi<float>(); };
  };
  FloatOp pi() {
    return [](float) { return glm::pi<float>(); };
  };
  FloatOp twoPi() {
    return [](float) { return glm::two_pi<float>(); };
  };
  FloatOp appWidth() {
    return [](float) { return ofGetWidth(); };
  };
  FloatOp appHeight() {
    return [](float) { return ofGetHeight(); };
  };
  FloatOp frameNum() {
    return [](float) { return ofGetFrameNum(); };
  };
  FloatOp mouseX() {
    return [](float) { return ofGetMouseX(); };
  };
  FloatOp mouseY() {
    return [](float) { return ofGetMouseY(); };
  };

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
  FloatOp asin();
  FloatOp cos(FloatOp fb);
  FloatOp cos(float fb);
  FloatOp cos();
  FloatOp acos();
  FloatOp tan(FloatOp fb);
  FloatOp tan(float fb);
  FloatOp tan();
  FloatOp pulse(FloatOp w);
  FloatOp pulse(float w);
  FloatOp pulse();
  FloatOp square();

  FloatOp easeIn(FloatOp e);
  FloatOp easeIn();
  FloatOp easeIn(float e);
  FloatOp easeOut(FloatOp e);
  FloatOp easeOut();
  FloatOp easeOut(float e);
  FloatOp easeInOut(FloatOp e);
  FloatOp easeInOut();
  FloatOp easeInOut(float e);
  FloatOp easeOutIn(FloatOp e);
  FloatOp easeOutIn();
  FloatOp easeOutIn(float e);

  FloatOp gaussian(FloatOp lo, FloatOp hi);
  FloatOp gaussian(FloatOp hi) { return gaussian(nullptr, hi); };
  FloatOp guassian(float hi) { return gaussian(c(hi)); };
  FloatOp gaussian() { return gaussian(nullptr, nullptr); };

  FloatOp random(FloatOp lo, FloatOp hi, FloatOp mode);
  FloatOp random() { return random(nullptr, nullptr, nullptr); };
  FloatOp random(FloatOp hi) { return random(nullptr, hi, nullptr); };
  FloatOp random(float hi) { return random(c(hi)); };
  FloatOp perlin(FloatOp x, FloatOp y = nullptr, FloatOp z = nullptr,
                 FloatOp falloff = nullptr, FloatOp octaves = nullptr);

  FloatOp mult(FloatOp op, float scalar);
  FloatOp bias(FloatOp op, float offset);
  FloatOp bias(FloatOp op, FloatOp offset);
  FloatOp phase(FloatOp op, float phaseOffset);
  FloatOp phase(FloatOp op, FloatOp phaseOffset);
  FloatOp rate(FloatOp op, float rateOffset);
  FloatOp rate(FloatOp op, FloatOp rateOffset);
  FloatOp ring(FloatOp opA, FloatOp opB);
  FloatOp fold(FloatOp op, FloatOp threshold);
  FloatOp fold(FloatOp op, float threshold);
  FloatOp fold(FloatOp op);
  FloatOp lpf(FloatOp inputOp, int windowSize);
  FloatOp chain(vector<FloatOp> ops);
  FloatOp choose(vector<FloatOp> ops);

  vector<float> normalize(vector<float> values);
  FloatOp timeseries(vector<float> yValues);

  vector<float> floatArray(FloatOp op, int numSamples, FloatOp mapOp = nullptr);
  vector<glm::vec2> glv2Array(FloatOp curve, float start, float end,
                              int numPoints, float yScale = 1.0f);
  vector<glm::vec3> glv3Array(FloatOp curve, float start, float end,
                              int numPoints, float yScale = 1.0f);
  vector<ofVec2f> ofv2Array(FloatOp curve, float start, float end,
                            int numPoints, float yScale = 1.0f);
  vector<ofVec3f> ofv3Array(FloatOp curve, float start, float end,
                            int numPoints, float yScale = 1.0f);

  float triDist(float lo, float hi, float mode);
  float pNoise(float x, float y, float z, float falloff = 1.f, int octaves = 1);
  float pNoise(float x, float y, float falloff = 1.f, int octaves = 1);
  float pNoise(float x, float falloff = 1.f, int octaves = 1);

  vector<float> table;
  FloatOp wavetable();

  void plot(FloatOp op, float yScale, ofColor color = ofColor::white,
            bool fill = false);
};
} // namespace ofxCrvs
