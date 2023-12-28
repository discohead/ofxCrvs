
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

  [[nodiscard]] FloatOp zero() const {
    return [](float) { return 0.0f; };
  };
  [[nodiscard]] FloatOp fourth() const {
    return [](float) { return 0.25f; };
  };
  [[nodiscard]] FloatOp third() const {
    return [](float) { return 1.f / 3.f; };
  };
  [[nodiscard]] FloatOp half() const {
    return [](float) { return 0.5f; };
  };
  [[nodiscard]] FloatOp one() const {
    return [](float) { return 1.0f; };
  };
  [[nodiscard]] FloatOp two() const {
    return [](float) { return 2.0f; };
  };
  [[nodiscard]] FloatOp three() const {
    return [](float) { return 3.0f; };
  };
  [[nodiscard]] FloatOp four() const {
    return [](float) { return 4.0f; };
  };
  [[nodiscard]] FloatOp quarterPi() const {
    return [](float) { return glm::quarter_pi<float>(); };
  };
  [[nodiscard]] FloatOp thirdPi() const {
    return [](float) { return glm::pi<float>() / 3.f; };
  };
  [[nodiscard]] FloatOp halfPi() const {
    return [](float) { return glm::half_pi<float>(); };
  };
  [[nodiscard]] FloatOp pi() const {
    return [](float) { return glm::pi<float>(); };
  };
  [[nodiscard]] FloatOp twoPi() const {
    return [](float) { return glm::two_pi<float>(); };
  };
  [[nodiscard]] FloatOp appWidth() const {
    return [](float) { return ofGetWidth(); };
  };
  [[nodiscard]] FloatOp appHeight() const {
    return [](float) { return ofGetHeight(); };
  };
  [[nodiscard]] FloatOp frameNum() const {
    return [](float) { return ofGetFrameNum(); };
  };
  [[nodiscard]] FloatOp mouseX() const {
    return [](float) { return ofGetMouseX(); };
  };
  [[nodiscard]] FloatOp mouseY() const {
    return [](float) { return ofGetMouseY(); };
  };

  static float pos2Rad(float pos);

  [[nodiscard]] FloatOp bipolarize(const FloatOp &unipolarOp) const;
  [[nodiscard]] FloatOp rectify(const FloatOp &bipolarOp) const;
  [[nodiscard]] FloatOp c(float value) const;
  [[nodiscard]] FloatOp phasor() const;
  [[nodiscard]] FloatOp saw() const;
  [[nodiscard]] FloatOp tri(const FloatOp &s) const;
  [[nodiscard]] FloatOp tri() const;
  [[nodiscard]] FloatOp tri(float s) const;
  [[nodiscard]] FloatOp sine(const FloatOp &fb) const;
  [[nodiscard]] FloatOp sine() const;
  [[nodiscard]] FloatOp sine(float fb) const;
  [[nodiscard]] FloatOp asin() const;
  [[nodiscard]] FloatOp cos(const FloatOp &fb) const;
  [[nodiscard]] FloatOp cos(float fb) const;
  [[nodiscard]] FloatOp cos() const;
  [[nodiscard]] FloatOp acos() const;
  [[nodiscard]] FloatOp tan(const FloatOp &fb) const;
  [[nodiscard]] FloatOp tan(float fb) const;
  [[nodiscard]] FloatOp tan() const;

  vector<float> table;
  [[nodiscard]] FloatOp wavetable() const;

  [[nodiscard]] FloatOp easeIn(const FloatOp &e) const;
  [[nodiscard]] FloatOp easeIn() const;
  [[nodiscard]] FloatOp easeIn(float e) const;
  [[nodiscard]] FloatOp easeOut(const FloatOp &e) const;
  [[nodiscard]] FloatOp easeOut() const;
  [[nodiscard]] FloatOp easeOut(float e) const;
  [[nodiscard]] FloatOp easeInOut(const FloatOp &e) const;
  [[nodiscard]] FloatOp easeInOut() const;
  [[nodiscard]] FloatOp easeInOut(float e) const;
  [[nodiscard]] FloatOp easeOutIn(const FloatOp &e) const;
  [[nodiscard]] FloatOp easeOutIn() const;
  [[nodiscard]] FloatOp easeOutIn(float e) const;

  // Envelope Ops
  [[nodiscard]] FloatOp env(float attackLength, float attackLevel,
                            float decayLength, float sustainLength,
                            float sustainLevel, float releaseLength) const;

  [[nodiscard]] FloatOp breakpoints(const vector<vector<float>> &points) const;
  [[nodiscard]] FloatOp timeseries(const vector<float> &yValues) const;

  [[nodiscard]] FloatOp gaussian(const FloatOp &lo, const FloatOp &hi) const;
  [[nodiscard]] FloatOp gaussian(const FloatOp &hi) const {
    return gaussian(FloatOp(), hi);
  };
  [[nodiscard]] FloatOp guassian(float hi) const { return gaussian(c(hi)); };
  [[nodiscard]] FloatOp gaussian() const {
    return gaussian(FloatOp(), FloatOp());
  };

  [[nodiscard]] FloatOp random(const FloatOp &lo, const FloatOp &hi,
                               const FloatOp &mode) const;
  [[nodiscard]] FloatOp random() const {
    return random(FloatOp(), FloatOp(), FloatOp());
  };
  [[nodiscard]] FloatOp random(const FloatOp &hi) const {
    return random(FloatOp(), hi, FloatOp());
  };
  [[nodiscard]] FloatOp random(float hi) const { return random(c(hi)); };
  [[nodiscard]] FloatOp perlin(const FloatOp &x, const FloatOp &y = FloatOp(),
                               const FloatOp &z = FloatOp(),
                               const FloatOp &falloff = FloatOp(),
                               const FloatOp &octaves = FloatOp()) const;

  // Basic ops
  [[nodiscard]] FloatOp abs(const FloatOp &op) const;
  [[nodiscard]] FloatOp diff(const FloatOp &opA, const FloatOp &opB) const;
  [[nodiscard]] FloatOp mult(const FloatOp &op, float scalar) const;
  [[nodiscard]] FloatOp bias(const FloatOp &op, float offset) const;
  [[nodiscard]] FloatOp bias(const FloatOp &op, const FloatOp &offset) const;
  [[nodiscard]] FloatOp phase(const FloatOp &op, float phaseOffset) const;
  [[nodiscard]] FloatOp phase(const FloatOp &op,
                              const FloatOp &phaseOffset) const;
  [[nodiscard]] FloatOp rate(const FloatOp &op, float rateOffset) const;
  [[nodiscard]] FloatOp rate(const FloatOp &op,
                             const FloatOp &rateOffset) const;
  [[nodiscard]] FloatOp ring(const FloatOp &opA, const FloatOp &opB) const;
  [[nodiscard]] FloatOp fold(const FloatOp &op, const FloatOp &threshold) const;
  [[nodiscard]] FloatOp fold(const FloatOp &op, float threshold) const;
  [[nodiscard]] FloatOp fold(const FloatOp &op) const;
  [[nodiscard]] FloatOp wrap(const FloatOp &op, float min, float max) const;
  [[nodiscard]] FloatOp wrap(const FloatOp &op, const FloatOp &minOp,
                             const FloatOp &maxOp) const;
  [[nodiscard]] FloatOp lpf(const FloatOp &inputOp, int windowSize) const;
  [[nodiscard]] FloatOp morph(const FloatOp &opA, const FloatOp &opB,
                              const FloatOp &morphParam) const;

  // Vector Ops - accept arrays of FloatOps
  [[nodiscard]] FloatOp chain(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp choose(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp mix(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp mix(const vector<FloatOp> &ops,
                            const vector<float> &levels) const;
  [[nodiscard]] FloatOp mix(const vector<FloatOp> &ops,
                            const vector<FloatOp> &levels) const;
  [[nodiscard]] FloatOp sum(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp product(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp min(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp max(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp mean(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp median(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp variance(const vector<FloatOp> &ops) const;
  [[nodiscard]] FloatOp stdDev(const vector<FloatOp> &ops) const;

  [[nodiscard]] FloatOp smooth() const;
  [[nodiscard]] FloatOp smoother() const;
  [[nodiscard]] FloatOp ema(float smoothingFactor) const;
  [[nodiscard]] FloatOp ema(const FloatOp &smoothingFactor) const;

  // Digital Ops - return 0 or 1
  [[nodiscard]] FloatOp pulse(const FloatOp &w = FloatOp()) const;
  [[nodiscard]] FloatOp pulse(float w) const;
  [[nodiscard]] FloatOp square() const;
  [[nodiscard]] FloatOp greater(const FloatOp &opA, const FloatOp &opB) const;
  [[nodiscard]] FloatOp greater(const FloatOp &opA, float threshold) const;
  [[nodiscard]] FloatOp less(const FloatOp &opA, const FloatOp &opB) const;
  [[nodiscard]] FloatOp less(const FloatOp &opA, float threshold) const;
  [[nodiscard]] FloatOp equal(const FloatOp &opA, const FloatOp &opB) const;
  [[nodiscard]] FloatOp equal(const FloatOp &opA, float threshold) const;
  [[nodiscard]] FloatOp notEqual(const FloatOp &opA, const FloatOp &opB) const;
  [[nodiscard]] FloatOp notEqual(const FloatOp &opA, float threshold) const;
  [[nodiscard]] FloatOp and_(const FloatOp &opA, const FloatOp &opB,
                             float threshold = 0.5f) const;
  [[nodiscard]] FloatOp and_(const FloatOp &opA, const FloatOp &opB,
                             const FloatOp &threshold) const;
  [[nodiscard]] FloatOp or_(const FloatOp &opA, const FloatOp &opB,
                            float threshold = 0.5f) const;
  [[nodiscard]] FloatOp or_(const FloatOp &opA, const FloatOp &opB,
                            const FloatOp &threshold) const;
  [[nodiscard]] FloatOp not_(const FloatOp &op) const;
  [[nodiscard]] FloatOp xor_(const FloatOp &opA, const FloatOp &opB,
                             float threshold = 0.5f) const;
  [[nodiscard]] FloatOp xor_(const FloatOp &opA, const FloatOp &opB,
                             const FloatOp &threshold) const;
  [[nodiscard]] FloatOp nand(const FloatOp &opA, const FloatOp &opB,
                             float threshold = 0.5f) const;
  [[nodiscard]] FloatOp nand(const FloatOp &opA, const FloatOp &opB,
                             const FloatOp &threshold) const;
  [[nodiscard]] FloatOp nor(const FloatOp &opA, const FloatOp &opB,
                            float threshold = 0.5f) const;
  [[nodiscard]] FloatOp nor(const FloatOp &opA, const FloatOp &opB,
                            const FloatOp &threshold) const;
  [[nodiscard]] FloatOp xnor(const FloatOp &opA, const FloatOp &opB,
                             float threshold = 0.5f) const;
  [[nodiscard]] FloatOp xnor(const FloatOp &opA, const FloatOp &opB,
                             const FloatOp &threshold) const;
  [[nodiscard]] FloatOp in(const FloatOp &op, float lo, float hi) const;
  [[nodiscard]] FloatOp in(const FloatOp &op, const FloatOp &lo,
                           const FloatOp &hi) const;
  [[nodiscard]] FloatOp in(const FloatOp &op, float lo,
                           const FloatOp &hi) const;
  [[nodiscard]] FloatOp in(const FloatOp &op, const FloatOp &lo,
                           float hi) const;
  [[nodiscard]] FloatOp out(const FloatOp &op, float lo, float hi) const;
  [[nodiscard]] FloatOp out(const FloatOp &op, const FloatOp &lo,
                            const FloatOp &hi) const;
  [[nodiscard]] FloatOp out(const FloatOp &op, float lo,
                            const FloatOp &hi) const;
  [[nodiscard]] FloatOp out(const FloatOp &op, const FloatOp &lo,
                            float hi) const;
  // End Digital Ops

  [[nodiscard]] vector<float> normalize(const vector<float> &values) const;

  [[nodiscard]] vector<float>
  floatArray(const FloatOp &op, int numSamples,
             const FloatOp &mapOp = FloatOp()) const;

  [[nodiscard]] vector<glm::vec2> glv2Array(const FloatOp &curve, float start,
                                            float end, int numPoints,
                                            float yScale = 1.0f) const;
  [[nodiscard]] vector<glm::vec3> glv3Array(const FloatOp &curve, float start,
                                            float end, int numPoints,
                                            float yScale = 1.0f) const;
  [[nodiscard]] vector<ofVec2f> ofv2Array(const FloatOp &curve, float start,
                                          float end, int numPoints,
                                          float yScale = 1.0f) const;
  [[nodiscard]] vector<ofVec3f> ofv3Array(const FloatOp &curve, float start,
                                          float end, int numPoints,
                                          float yScale = 1.0f) const;

  [[nodiscard]] float triDist(float lo, float hi, float mode) const;
  [[nodiscard]] float pNoise(float x, float y, float z, float falloff = 1.f,
                             int octaves = 1) const;
  [[nodiscard]] float pNoise(float x, float y, float falloff = 1.f,
                             int octaves = 1) const;
  [[nodiscard]] float pNoise(float x, float falloff = 1.f,
                             int octaves = 1) const;

  void plot(const FloatOp &op, float yScale, ofColor color = ofColor::white,
            bool fill = false) const;
};
} // namespace ofxCrvs
