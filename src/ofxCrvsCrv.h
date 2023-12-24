
#pragma once

#include "ofxCrvsBox.hpp"
#include "ofxCrvsEdg.hpp"
#include "ofxCrvsOps.h"

namespace ofxCrvs {
class Edg;
/**
 * The enum Component.
 */
enum class Component {
  /**
   * X component.
   */
  X,

  /**
   * Y component.
   */
  Y,
  /**
   * Z Component.
   */
  Z
};

enum class Bounding {
  NONE,
  CLIPPING,
  WRAPPING,
  FOLDING,
};

class Crv {
 public:
  constexpr static glm::vec3 uCenter = glm::vec3(0.5f);

  Box box;
  FloatOp op;
  std::shared_ptr<Crv> amp;
  std::shared_ptr<Crv> rate;
  std::shared_ptr<Crv> phase;
  std::shared_ptr<Crv> bias;

  float ampOffset = 1.0f;
  float rateOffset = 1.0f;
  float phaseOffset = 0.0f;
  float biasOffset = 0.0f;

  int resolution;
  int quantization;
  glm::vec3 origin;
  glm::vec3 translation;
  glm::vec3 scale;
  float rotation;

  Bounding bounding;

  static std::shared_ptr<Crv> create() { return std::make_shared<Crv>(); }

  static std::shared_ptr<Crv> create(FloatOp op) {
    return std::make_shared<Crv>(op);
  }

  static std::shared_ptr<Crv> create(FloatOp op, std::shared_ptr<Crv> amp,
                                     std::shared_ptr<Crv> rate,
                                     std::shared_ptr<Crv> phase,
                                     std::shared_ptr<Crv> bias) {
    return std::make_shared<Crv>(op, amp, rate, phase, bias);
  }

  static std::shared_ptr<Crv> create(FloatOp op, float ampOffset,
                                     float rateOffset, float phaseOffset,
                                     float biasOffset) {
    return std::make_shared<Crv>(op, ampOffset, rateOffset, phaseOffset,
                                 biasOffset);
  }

  static std::shared_ptr<Crv> create(
      Box box, FloatOp op, std::shared_ptr<Crv> amp, std::shared_ptr<Crv> rate,
      std::shared_ptr<Crv> phase, std::shared_ptr<Crv> bias, float ampOffset,
      float rateOffset, float phaseOffset, float biasOffset) {
    return std::make_shared<Crv>(box, op, amp, rate, phase, bias, ampOffset,
                                 rateOffset, phaseOffset, biasOffset);
  }

  Crv() : Crv(nullptr, nullptr, nullptr, nullptr, nullptr) {}

  Crv(Box box)
      : Crv(box, nullptr, nullptr, nullptr, nullptr, nullptr, 1.0f, 1.0f, 0.0f,
            0.0f) {}

  Crv(FloatOp op) : Crv(op, nullptr, nullptr, nullptr, nullptr) {}

  Crv(FloatOp op, std::shared_ptr<Crv> amp, std::shared_ptr<Crv> rate,
      std::shared_ptr<Crv> phase, std::shared_ptr<Crv> bias)
      : Crv(Box(ofGetWidth(), ofGetHeight(), 0.f), op, amp, rate, phase, bias,
            1.0f, 1.0f, 0.0f, 0.0f) {}

  Crv(FloatOp op, float ampOffset, float rateOffset, float phaseOffset,
      float biasOffset)
      : Crv(Box(ofGetWidth(), ofGetHeight(), 0.f), op, nullptr, nullptr,
            nullptr, nullptr, ampOffset, rateOffset, phaseOffset, biasOffset) {}

  Crv(Box box, FloatOp op, std::shared_ptr<Crv> amp, std::shared_ptr<Crv> rate,
      std::shared_ptr<Crv> phase, std::shared_ptr<Crv> bias, float ampOffset,
      float rateOffset, float phaseOffset, float biasOffset)
      : box(box),
        op(op),
        amp(amp),
        rate(rate),
        phase(phase),
        bias(bias),
        ampOffset(ampOffset),
        rateOffset(rateOffset),
        phaseOffset(phaseOffset),
        biasOffset(biasOffset) {
    resolution = box.getWidth();
    quantization = 0;
    origin = glm::vec3(0.f);
    translation = glm::vec3(0.f);
    scale = glm::vec3(1.f);
    rotation = 0.0f;
    if (!op) this->op = [](float x) { return x; };
  }

  virtual ~Crv() = default;

  float apply(float pos) const;
  float xAt(float pos) const;
  float yAt(float pos) const;
  float zAt(float pos) const;
  float bipolarize(float unipolar) const;
  float wrap(float value, float min, float max) const;
  float fold(float value, float min, float max) const;
  float rectify(float bipolar) const;

  std::vector<float> floatArray(int numSamples, Component component) const;
  std::vector<float> floatArray(int numSamples) const;
  std::vector<std::vector<float>> coordinateArray(int numPoints, bool boxed,
                                                  bool transformed,
                                                  FloatOp samplingRateOp) const;
  std::vector<glm::vec2> glv2Array(int numPoints, bool boxed, bool transformed,
                                   FloatOp samplingRateOp) const;
  std::vector<glm::vec3> glv3Array(int numPoints, bool boxed, bool transformed,
                                   FloatOp samplingRateOp) const;
  std::vector<ofVec3f> ofv3Array(int numPoints, bool boxed, bool transformed,
                                 FloatOp samplingRateOp) const;
  std::vector<ofVec2f> ofv2Array(int numPoints, bool boxed, bool transformed,
                                 FloatOp samplingRateOp) const;
  ofPolyline polyline(int numPoints, bool boxed, bool transformed,
                      FloatOp samplingRateOp) const;

  glm::vec3 uVector(float pos, bool transformed) const;
  glm::vec3 wVector(float pos, bool transformed) const;

  void boxed(glm::vec3& v) const;
  void transform(glm::vec3& v) const;
  void bounded(glm::vec3& v) const;
  void wrapped(glm::vec3& v) const;
  void folded(glm::vec3& v) const;

  std::vector<Edg> getWebEdgs(int numPoints, bool boxed, bool transformed,
                              int resolution) const;
  std::vector<Edg> getWebEdgs(int numPoints, bool boxed,
                              bool transformed) const;

 protected:
  float calculate(float pos) const;
  float ampBias(float value, float pos) const;
  float calcPos(float pos) const;
  virtual float componentAt(Component component, float pos) const;
  float quantize(float y) const;
};
}  // namespace ofxCrvs
