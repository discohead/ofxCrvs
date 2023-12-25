
#pragma once

#include <functional>

#include "ofMain.h"

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

  FloatOp zero() const {
    return [](float) { return 0.0f; };
  };
  FloatOp fourth() const {
    return [](float) { return 0.25f; };
  };
  FloatOp third() const {
    return [](float) { return 1.f / 3.f; };
  };
  FloatOp half() const {
    return [](float) { return 0.5f; };
  };
  FloatOp one() const {
    return [](float) { return 1.0f; };
  };
  FloatOp two() const {
    return [](float) { return 2.0f; };
  };
  FloatOp three() const {
    return [](float) { return 3.0f; };
  };
  FloatOp four() const {
    return [](float) { return 4.0f; };
  };
  FloatOp quarterPi() const {
    return [](float) { return glm::quarter_pi<float>(); };
  };
  FloatOp thirdPi() const {
    return [](float) { return glm::pi<float>() / 3.f; };
  };
  FloatOp halfPi() const {
    return [](float) { return glm::half_pi<float>(); };
  };
  FloatOp pi() const {
    return [](float) { return glm::pi<float>(); };
  };
  FloatOp twoPi() const {
    return [](float) { return glm::two_pi<float>(); };
  };
  FloatOp appWidth() const {
    return [](float) { return ofGetWidth(); };
  };
  FloatOp appHeight() const {
    return [](float) { return ofGetHeight(); };
  };
  FloatOp frameNum() const {
    return [](float) { return ofGetFrameNum(); };
  };
  FloatOp mouseX() const {
    return [](float) { return ofGetMouseX(); };
  };
  FloatOp mouseY() const {
    return [](float) { return ofGetMouseY(); };
  };

  static float pos2Rad(float pos);
  FloatOp bipolarize(FloatOp unipolarOp) const;
  FloatOp rectify(FloatOp bipolarOp) const;
  FloatOp c(float value) const;
  FloatOp phasor() const;
  FloatOp saw() const;
  FloatOp tri(FloatOp s) const;
  FloatOp tri() const;
  FloatOp tri(float s) const;
  FloatOp sine(FloatOp fb) const;
  FloatOp sine() const;
  FloatOp sine(float fb) const;
  FloatOp asin() const;
  FloatOp cos(FloatOp fb) const;
  FloatOp cos(float fb) const;
  FloatOp cos() const;
  FloatOp acos() const;
  FloatOp tan(FloatOp fb) const;
  FloatOp tan(float fb) const;
  FloatOp tan() const;
  FloatOp pulse(FloatOp w) const;
  FloatOp pulse(float w) const;
  FloatOp pulse() const;
  FloatOp square() const;

  FloatOp easeIn(FloatOp e) const;
  FloatOp easeIn() const;
  FloatOp easeIn(float e) const;
  FloatOp easeOut(FloatOp e) const;
  FloatOp easeOut() const;
  FloatOp easeOut(float e) const;
  FloatOp easeInOut(FloatOp e) const;
  FloatOp easeInOut() const;
  FloatOp easeInOut(float e) const;
  FloatOp easeOutIn(FloatOp e) const;
  FloatOp easeOutIn() const;
  FloatOp easeOutIn(float e) const;

  FloatOp gaussian(FloatOp lo, FloatOp hi) const;
  FloatOp gaussian(FloatOp hi) const { return gaussian(nullptr, hi); };
  FloatOp guassian(float hi) const { return gaussian(c(hi)); };
  FloatOp gaussian() const { return gaussian(nullptr, nullptr); };

  FloatOp random(FloatOp lo, FloatOp hi, FloatOp mode) const;
  FloatOp random() const { return random(nullptr, nullptr, nullptr); };
  FloatOp random(FloatOp hi) const { return random(nullptr, hi, nullptr); };
  FloatOp random(float hi) const { return random(c(hi)); };
  FloatOp perlin(FloatOp x, FloatOp y = nullptr, FloatOp z = nullptr,
                 FloatOp falloff = nullptr, FloatOp octaves = nullptr) const;

  FloatOp mult(FloatOp op, float scalar) const;
  FloatOp bias(FloatOp op, float offset) const;
  FloatOp bias(FloatOp op, FloatOp offset) const;
  FloatOp phase(FloatOp op, float phaseOffset) const;
  FloatOp phase(FloatOp op, FloatOp phaseOffset) const;
  FloatOp rate(FloatOp op, float rateOffset) const;
  FloatOp rate(FloatOp op, FloatOp rateOffset) const;
  FloatOp ring(FloatOp opA, FloatOp opB) const;
  FloatOp fold(FloatOp op, FloatOp threshold) const;
  FloatOp fold(FloatOp op, float threshold) const;
  FloatOp fold(FloatOp op) const;
  FloatOp lpf(FloatOp inputOp, int windowSize) const;

  FloatOp chain(vector<FloatOp> ops) const;
  FloatOp choose(vector<FloatOp> ops) const;
  FloatOp mix(vector<FloatOp> ops) const;
  FloatOp mix(vector<FloatOp> ops, vector<float> levels) const;
  FloatOp sum(vector<FloatOp> ops) const;
  FloatOp product(vector<FloatOp> ops) const;
  FloatOp min(vector<FloatOp> ops) const;
  FloatOp max(vector<FloatOp> ops) const;
  FloatOp mean(vector<FloatOp> ops) const;
  FloatOp median(vector<FloatOp> ops) const;
  FloatOp variance(vector<FloatOp> ops) const;
  FloatOp stdDev(vector<FloatOp> ops) const;
  
  FloatOp abs(FloatOp op) const;
  FloatOp diff(FloatOp opA, FloatOp opB) const;
  FloatOp greater(FloatOp opA, FloatOp opB) const;
  FloatOp greater(FloatOp opA, float threshold) const;
  FloatOp less(FloatOp opA, FloatOp opB) const;
  FloatOp less(FloatOp opA, float threshold) const;
  FloatOp equal(FloatOp opA, FloatOp opB) const;
  FloatOp equal(FloatOp opA, float threshold) const;
  FloatOp notEqual(FloatOp opA, FloatOp opB) const;
  FloatOp notEqual(FloatOp opA, float threshold) const;
  FloatOp and_(FloatOp opA, FloatOp opB, float threshold = 0.5f) const;
  FloatOp and_(FloatOp opA, FloatOp opB, FloatOp threshold) const;
  FloatOp or_(FloatOp opA, FloatOp opB, float threshold = 0.5f) const;
  FloatOp or_(FloatOp opA, FloatOp opB, FloatOp threshold) const;
  FloatOp not_(FloatOp op) const;
  FloatOp xor_(FloatOp opA, FloatOp opB, float threhold = 0.5f) const;
  FloatOp xor_(FloatOp opA, FloatOp opB, FloatOp threhold) const;
  FloatOp nand(FloatOp opA, FloatOp opB, float threshold = 0.5f) const;
  FloatOp nand(FloatOp opA, FloatOp opB, FloatOp threshold) const;
  FloatOp nor(FloatOp opA, FloatOp opB, float threshold = 0.5f) const;
  FloatOp nor(FloatOp opA, FloatOp opB, FloatOp threshold) const;
  FloatOp xnor(FloatOp opA, FloatOp opB, float threshold = 0.5f) const;
  FloatOp xnor(FloatOp opA, FloatOp opB, FloatOp threshold) const;
  FloatOp in(FloatOp op, float lo, float hi) const;
  FloatOp in(FloatOp op, FloatOp lo, FloatOp hi) const;
  FloatOp in(FloatOp op, float lo, FloatOp hi) const;
  FloatOp in(FloatOp op, FloatOp lo, float hi) const;
  FloatOp out(FloatOp op, float lo, float hi) const;
  FloatOp out(FloatOp op, FloatOp lo, FloatOp hi) const;
  FloatOp out(FloatOp op, float lo, FloatOp hi) const;
  FloatOp out(FloatOp op, FloatOp lo, float hi) const;

  vector<float> normalize(vector<float> values) const;
  FloatOp timeseries(vector<float> yValues) const;

  vector<float> floatArray(FloatOp op, int numSamples,
                           FloatOp mapOp = nullptr) const;
  vector<glm::vec2> glv2Array(FloatOp curve, float start, float end,
                              int numPoints, float yScale = 1.0f) const;
  vector<glm::vec3> glv3Array(FloatOp curve, float start, float end,
                              int numPoints, float yScale = 1.0f) const;
  vector<ofVec2f> ofv2Array(FloatOp curve, float start, float end,
                            int numPoints, float yScale = 1.0f) const;
  vector<ofVec3f> ofv3Array(FloatOp curve, float start, float end,
                            int numPoints, float yScale = 1.0f) const;

  float triDist(float lo, float hi, float mode) const;
  float pNoise(float x, float y, float z, float falloff = 1.f,
               int octaves = 1) const;
  float pNoise(float x, float y, float falloff = 1.f, int octaves = 1) const;
  float pNoise(float x, float falloff = 1.f, int octaves = 1) const;

  vector<float> table;
  FloatOp wavetable() const;

  void plot(FloatOp op, float yScale, ofColor color = ofColor::white,
            bool fill = false) const;
};
}  // namespace ofxCrvs
