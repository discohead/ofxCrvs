
#pragma once

#include <functional>

#include "ofMain.h"

namespace ofxCrvs {

using FloatOp = std::function<float(const float)>;

class Ops {
public:
  Ops() {
    table.resize(1024);
    for (int i = 0; i < 1024; i++) {
      table[i] = ofRandom(1.0f);
    }
  };
  explicit Ops(const vector<float> &table) : table(table){};

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

  static float pos2Rad(const float pos);

  FloatOp bipolarize(const FloatOp &unipolarOp) const;
  FloatOp rectify(const FloatOp &bipolarOp) const;
  FloatOp c(const float value) const;
  FloatOp phasor() const;
  FloatOp saw() const;
  FloatOp tri(const FloatOp &s) const;
  FloatOp tri() const;
  FloatOp tri(const float s) const;
  FloatOp sine(const FloatOp &fb) const;
  FloatOp sine() const;
  FloatOp sine(const float fb) const;
  FloatOp asin() const;
  FloatOp cos(const FloatOp &fb) const;
  FloatOp cos(const float fb) const;
  FloatOp cos() const;
  FloatOp acos() const;
  FloatOp tan(const FloatOp &fb) const;
  FloatOp tan(const float fb) const;
  FloatOp tan() const;

  FloatOp easeIn(const FloatOp &e) const;
  FloatOp easeIn() const;
  FloatOp easeIn(const float e) const;
  FloatOp easeOut(const FloatOp &e) const;
  FloatOp easeOut() const;
  FloatOp easeOut(const float e) const;
  FloatOp easeInOut(const FloatOp &e) const;
  FloatOp easeInOut() const;
  FloatOp easeInOut(const float e) const;
  FloatOp easeOutIn(const FloatOp &e) const;
  FloatOp easeOutIn() const;
  FloatOp easeOutIn(const float e) const;

  // Envelope Ops
  FloatOp env(const float attackLength, const float attackLevel,
              const float decayLength, float sustainLength,
              const float sustainLevel, float releaseLength) const;

  FloatOp breakpoints(const vector<vector<float>> &points) const;

  FloatOp gaussian(const FloatOp &lo, const FloatOp &hi) const;
  FloatOp gaussian(const FloatOp &hi) const { return gaussian(FloatOp(), hi); };
  FloatOp guassian(const float hi) const { return gaussian(c(hi)); };
  FloatOp gaussian() const { return gaussian(FloatOp(), FloatOp()); };

  FloatOp random(const FloatOp &lo, const FloatOp &hi,
                 const FloatOp &mode) const;
  FloatOp random() const { return random(FloatOp(), FloatOp(), FloatOp()); };
  FloatOp random(const FloatOp &hi) const {
    return random(FloatOp(), hi, FloatOp());
  };
  FloatOp random(const float hi) const { return random(c(hi)); };
  FloatOp perlin(const FloatOp &x, const FloatOp &y = FloatOp(),
                 const FloatOp &z = FloatOp(),
                 const FloatOp &falloff = FloatOp(),
                 const FloatOp &octaves = FloatOp()) const;

  // Basic ops
  FloatOp abs(const FloatOp &op) const;
  FloatOp diff(const FloatOp &opA, const FloatOp &opB) const;
  FloatOp mult(const FloatOp &op, const float scalar) const;
  FloatOp bias(const FloatOp &op, const float offset) const;
  FloatOp bias(const FloatOp &op, const FloatOp &offset) const;
  FloatOp phase(const FloatOp &op, const float phaseOffset) const;
  FloatOp phase(const FloatOp &op, const FloatOp &phaseOffset) const;
  FloatOp rate(const FloatOp &op, const float rateOffset) const;
  FloatOp rate(const FloatOp &op, const FloatOp &rateOffset) const;
  FloatOp ring(const FloatOp &opA, const FloatOp &opB) const;
  FloatOp fold(const FloatOp &op, const FloatOp &threshold) const;
  FloatOp fold(const FloatOp &op, const float threshold) const;
  FloatOp fold(const FloatOp &op) const;
  FloatOp lpf(const FloatOp &inputOp, const int windowSize) const;

  // Vector Ops - accept arrays of FloatOps
  FloatOp chain(const vector<FloatOp> &ops) const;
  FloatOp choose(const vector<FloatOp> &ops) const;
  FloatOp mix(const vector<FloatOp> &ops) const;
  FloatOp mix(const vector<FloatOp> &ops, const vector<float> &levels) const;
  FloatOp mix(const vector<FloatOp> &ops, const vector<FloatOp> &levels) const;
  FloatOp sum(const vector<FloatOp> &ops) const;
  FloatOp product(const vector<FloatOp> &ops) const;
  FloatOp min(const vector<FloatOp> &ops) const;
  FloatOp max(const vector<FloatOp> &ops) const;
  FloatOp mean(const vector<FloatOp> &ops) const;
  FloatOp median(const vector<FloatOp> &ops) const;
  FloatOp variance(const vector<FloatOp> &ops) const;
  FloatOp stdDev(const vector<FloatOp> &ops) const;

  // Digital Ops - return 0 or 1
  FloatOp pulse(const FloatOp &w = FloatOp()) const;
  FloatOp pulse(const float w) const;
  FloatOp square() const;
  FloatOp greater(const FloatOp &opA, const FloatOp &opB) const;
  FloatOp greater(const FloatOp &opA, const float threshold) const;
  FloatOp less(const FloatOp &opA, const FloatOp &opB) const;
  FloatOp less(const FloatOp &opA, const float threshold) const;
  FloatOp equal(const FloatOp &opA, const FloatOp &opB) const;
  FloatOp equal(const FloatOp &opA, const float threshold) const;
  FloatOp notEqual(const FloatOp &opA, const FloatOp &opB) const;
  FloatOp notEqual(const FloatOp &opA, const float threshold) const;
  FloatOp and_(const FloatOp &opA, const FloatOp &opB,
               const float threshold = 0.5f) const;
  FloatOp and_(const FloatOp &opA, const FloatOp &opB,
               const FloatOp &threshold) const;
  FloatOp or_(const FloatOp &opA, const FloatOp &opB,
              const float threshold = 0.5f) const;
  FloatOp or_(const FloatOp &opA, const FloatOp &opB,
              const FloatOp &threshold) const;
  FloatOp not_(const FloatOp &op) const;
  FloatOp xor_(const FloatOp &opA, const FloatOp &opB,
               const float threhold = 0.5f) const;
  FloatOp xor_(const FloatOp &opA, const FloatOp &opB,
               const FloatOp &threhold) const;
  FloatOp nand(const FloatOp &opA, const FloatOp &opB,
               const float threshold = 0.5f) const;
  FloatOp nand(const FloatOp &opA, const FloatOp &opB,
               const FloatOp &threshold) const;
  FloatOp nor(const FloatOp &opA, const FloatOp &opB,
              const float threshold = 0.5f) const;
  FloatOp nor(const FloatOp &opA, const FloatOp &opB,
              const FloatOp &threshold) const;
  FloatOp xnor(const FloatOp &opA, const FloatOp &opB,
               const float threshold = 0.5f) const;
  FloatOp xnor(const FloatOp &opA, const FloatOp &opB,
               const FloatOp &threshold) const;
  FloatOp in(const FloatOp &op, const float lo, const float hi) const;
  FloatOp in(const FloatOp &op, const FloatOp &lo, const FloatOp &hi) const;
  FloatOp in(const FloatOp &op, const float lo, const FloatOp &hi) const;
  FloatOp in(const FloatOp &op, const FloatOp &lo, const float hi) const;
  FloatOp out(const FloatOp &op, const float lo, const float hi) const;
  FloatOp out(const FloatOp &op, const FloatOp &lo, const FloatOp &hi) const;
  FloatOp out(const FloatOp &op, const float lo, const FloatOp &hi) const;
  FloatOp out(const FloatOp &op, const FloatOp &lo, const float hi) const;
  // End Digital Ops

  vector<float> normalize(vector<float> &values) const;
  FloatOp timeseries(vector<float> &yValues) const;

  vector<float> floatArray(const FloatOp &op, int numSamples,
                           const FloatOp &mapOp = FloatOp()) const;
  vector<glm::vec2> glv2Array(const FloatOp &curve, const float start,
                              const float end, int numPoints,
                              const float yScale = 1.0f) const;
  vector<glm::vec3> glv3Array(const FloatOp &curve, const float start,
                              const float end, int numPoints,
                              const float yScale = 1.0f) const;
  vector<ofVec2f> ofv2Array(const FloatOp &curve, const float start,
                            const float end, int numPoints,
                            const float yScale = 1.0f) const;
  vector<ofVec3f> ofv3Array(const FloatOp &curve, const float start,
                            const float end, int numPoints,
                            const float yScale = 1.0f) const;

  float triDist(const float lo, const float hi, const float mode) const;
  float pNoise(const float x, const float y, const float z,
               const float falloff = 1.f, int octaves = 1) const;
  float pNoise(const float x, const float y, const float falloff = 1.f,
               int octaves = 1) const;
  float pNoise(const float x, const float falloff = 1.f, int octaves = 1) const;

  vector<float> table;
  FloatOp wavetable() const;

  void plot(const FloatOp &op, const float yScale,
            ofColor color = ofColor::white, bool fill = false) const;
};
} // namespace ofxCrvs
