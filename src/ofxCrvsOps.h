
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
    return [this](float) { return ofGetWidth(); };
  };
  FloatOp appHeight() {
    return [this](float) { return ofGetHeight(); };
  };
  FloatOp frameNum() {
    return [this](float) { return ofGetFrameNum(); };
  };
  FloatOp mouseX() {
    return [this](float) { return ofGetMouseX(); };
  };
  FloatOp mouseY() {
    return [this](float) { return ofGetMouseY(); };
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

  float triDist(float lo, float hi, float mode);
  float pNoise(float x, float y, float z, float falloff = 1.f, int octaves = 1);
  float pNoise(float x, float y, float falloff = 1.f, int octaves = 1);
  float pNoise(float x, float falloff = 1.f, int octaves = 1);

  vector<float> table;
  FloatOp wavetable();

private:
  ofBaseApp *app = ofGetAppPtr();
};
} // namespace ofxCrvs
