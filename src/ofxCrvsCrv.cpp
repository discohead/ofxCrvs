
#include <algorithm>

#include "ofxCrvsCrv.h"
#include "ofxCrvsEdg.hpp"
#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

float Crv::calculate(float pos) { return quantize(op(pos)); }

float Crv::apply(float pos) { return calculate(pos); }

float Crv::ampBias(float value, float pos) {
  float ampFactor = ampOffset;
  if (amp != nullptr)
    ampFactor *= amp->yAt(pos);
  ampFactor = ampFactor / 2.f;
  value = value * ampFactor + ampFactor;
  if (bias != nullptr)
    value += bias->yAt(pos);
  return value + biasOffset;
}

float Crv::calcPos(float pos) {
  pos = std::abs(pos);
  pos *= rateOffset;
  if (pos > 1.f)
    pos = fmod(pos, 1.f);
  if (rate != nullptr)
    pos *= rate->yAt(pos);
  if (phase != nullptr)
    pos += phase->yAt(pos);
  pos += phaseOffset;
  if (pos > 1.f)
    pos = fmod(pos, 1.f);
  return pos;
}

float Crv::componentAt(Component component, float pos) {
  if (component == Component::X)
    return pos;
  float modPos = calcPos(pos);
  float value = calculate(modPos);
  value = bipolarize(value);
  value = ampBias(value, modPos);
  return value;
}

float Crv::yAt(float pos) { return componentAt(Component::Y, pos); }

float Crv::xAt(float pos) { return componentAt(Component::X, pos); }

float Crv::quantize(float y) {
  if (quantization > 1) {
    float levelSize = 1.f / (quantization - 1);
    int quantizedLevel = round(y / levelSize);
    return quantizedLevel * levelSize;
  }
  return y;
}

float Crv::bipolarize(float unipolar) { return 2.f * unipolar - 1.f; }

float Crv::rectify(float bipolar) { return bipolar * 0.5f + 0.5f; }

float Crv::wrap(float value, float min, float max) {
  float range = max - min;
  float wrappedValue = fmod(value - min, range);
  if (wrappedValue < 0.f) {
    wrappedValue += range;
  } else if (wrappedValue == 0 && value == max) {
    wrappedValue = range;
  }
  return wrappedValue + min;
}

float Crv::fold(float value, float min, float max) {
  float range = max - min;
  float foldedValue = fmod((value - min), (2 * range));
  if (foldedValue < 0)
    foldedValue += 2 * range;
  return max - std::abs(foldedValue - range);
}

std::vector<float> Crv::floatArray(int numSamples, Component component) {
  float step = 1.f / static_cast<float>(numSamples);
  std::vector<float> table(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    float x = i * step;
    table[i] = this->componentAt(component, x);
  }
  return table;
}

std::vector<float> Crv::floatArray(int numSamples) {
  return floatArray(numSamples, Component::Y);
}

std::vector<glm::vec2> Crv::glv2Array(int numPoints, bool windowed,
                                      bool transformed,
                                      FloatOp samplingRateOp) {
  std::vector<glm::vec2> points(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr)
      x = samplingRateOp(x);
    if (windowed)
      points[i] = wVector(x, transformed);
    else
      points[i] = uVector(x, transformed);
  }
  return points;
}

std::vector<glm::vec3> Crv::glv3Array(int numPoints, bool windowed,
                                      bool transformed,
                                      FloatOp samplingRateOp) {
  std::vector<glm::vec3> vectors(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr)
      x = samplingRateOp(x);
    if (windowed)
      vectors[i] = glm::vec3(wVector(x, transformed), 0.f);
    else
      vectors[i] = glm::vec3(uVector(x, transformed), 0.f);
    ;
  }
  return vectors;
}

std::vector<std::vector<float>> Crv::coordinateArray(int numPoints,
                                                     bool windowed,
                                                     bool transformed,
                                                     FloatOp samplingRateOp) {
  std::vector<glm::vec2> vectors =
      glv2Array(numPoints, windowed, transformed, samplingRateOp);
  std::vector<std::vector<float>> points(numPoints, std::vector<float>(2));
  for (int i = 0; i < numPoints; ++i) {
    points[i][0] = vectors[i].x;
    points[i][1] = vectors[i].y;
  }
  return points;
}

std::vector<ofVec3f> Crv::ofv3Array(int numPoints, bool windowed,
                                    bool transformed, FloatOp samplingRateOp) {
  std::vector<ofVec3f> vectors(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr)
      x = samplingRateOp(x);
    glm::vec2 v;
    if (windowed)
      v = wVector(x, transformed);
    else
      v = uVector(x, transformed);
    vectors[i] = ofVec3f(v.x, v.y, 0.f);
  }
  return vectors;
}

std::vector<ofVec2f> Crv::ofv2Array(int numPoints, bool windowed,
                                    bool transformed, FloatOp samplingRateOp) {
  std::vector<ofVec2f> vectors(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr)
      x = samplingRateOp(x);
    glm::vec2 v;
    if (windowed)
      v = wVector(x, transformed);
    else
      v = uVector(x, transformed);
    vectors[i] = ofVec2f(v.x, v.y);
  }
  return vectors;
}

ofPolyline Crv::polyline(int numPoints, bool windowed, bool transformed,
                         FloatOp samplingRateOp) {
  std::vector<glm::vec2> vectors =
      glv2Array(numPoints, windowed, transformed, samplingRateOp);
  ofPolyline polyline;
  for (const auto &v : vectors)
    polyline.addVertex(glm::vec3(v, 0.f));
  return polyline;
}

glm::vec2 Crv::uVector(float pos, bool transformed) {
  float x = xAt(pos);
  float y = yAt(pos);
  glm::vec2 v = glm::vec2(x, y);
  if (transformed)
    v = transform(v);
  v += origin;
  return bounded(v);
}

glm::vec2 Crv::wVector(float pos, bool transformed) {
  glm::vec2 v = uVector(pos, transformed);
  return windowed(v);
}

glm::vec2 Crv::transform(glm::vec2 v) {
  return Utils::transform(v, Crv::uCenter, scale, translation, rotation);
}

glm::vec2 Crv::bounded(glm::vec2 v) {
  switch (bounding) {
  case Bounding::CLIPPING:
    return clipped(v);
    break;
  case Bounding::WRAPPING:
    return wrapped(v);
    break;
  case Bounding::FOLDING:
    return folded(v);
    break;
  default:
    return v;
    break;
  }
}

glm::vec2 Crv::clipped(glm::vec2 v, float min, float max) {
  v.x = std::max(min, std::min(max, v.x));
  v.y = std::max(min, std::min(max, v.y));
  return v;
}

glm::vec2 Crv::clipped(glm::vec2 v) { return clipped(v, 0.f, 1.f); }

glm::vec2 Crv::wrapped(glm::vec2 v) {
  v.x = wrap(v.x, 0.f, 1.f);
  v.y = wrap(v.y, 0.f, 1.f);
  return v;
}

glm::vec2 Crv::folded(glm::vec2 v) {
  v.x = fold(v.x, 0.f, 1.f);
  v.y = fold(v.y, 0.f, 1.f);
  return v;
}

glm::vec2 Crv::windowed(glm::vec2 v) { return window.apply(v); }

std::vector<Edg> Crv::getWebEdgs(int numPoints, bool windowed, bool transformed,
                                 int resolution) {
  std::vector<glm::vec2> vectors =
      glv2Array(numPoints, windowed, transformed, nullptr);
  std::vector<Edg> edgs;
  for (int i = 0; i < vectors.size(); ++i) {
    for (int j = 0; j < vectors.size(); ++j) {
      if (i != j) {
        edgs.push_back(Edg(vectors[i], vectors[j], resolution));
      }
    }
  }
  return edgs;
}

std::vector<Edg> Crv::getWebEdgs(int numPoints, bool windowed,
                                 bool transformed) {
  return getWebEdgs(numPoints, windowed, transformed, resolution);
}

} // namespace ofxCrvs
