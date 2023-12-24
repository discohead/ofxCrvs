
#include "ofxCrvsCrv.h"

#include <algorithm>

#include "ofxCrvsConstants.h"
#include "ofxCrvsEdg.hpp"
#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

float Crv::calculate(float pos) const { return quantize(op(pos)); }

float Crv::apply(float pos) const { return calculate(pos); }

float Crv::ampBias(float value, float pos) const {
  float ampFactor = ampOffset;
  if (amp != nullptr) ampFactor *= amp->yAt(pos);
  ampFactor = ampFactor / 2.f;
  value = value * ampFactor + ampFactor;
  if (bias != nullptr) value += bias->yAt(pos);
  return value + biasOffset;
}

float Crv::calcPos(float pos) const {
  pos = abs(pos);
  pos *= rateOffset;
  if (pos > 1.f) pos = fmod(pos, 1.f);
  if (rate != nullptr) pos *= rate->yAt(pos);
  if (phase != nullptr) pos += phase->yAt(pos);
  pos += phaseOffset;
  if (pos > 1.f) pos = fmod(pos, 1.f);
  return pos;
}

float Crv::componentAt(Component component, float pos) const {
  if (component == Component::X) {
    return pos;
  } else if (component == Component::Y) {
    float modPos = calcPos(pos);
    float value = calculate(modPos);
    value = bipolarize(value);
    return ampBias(value, modPos);
  } else {
    return 0.f;
  }
}

float Crv::xAt(float pos) const { return componentAt(Component::X, pos); }

float Crv::yAt(float pos) const { return componentAt(Component::Y, pos); }

float Crv::zAt(float pos) const { return componentAt(Component::Z, pos); }

float Crv::quantize(float y) const {
  if (quantization > 1) {
    float levelSize = 1.f / (quantization - 1);
    int quantizedLevel = round(y / levelSize);
    return quantizedLevel * levelSize;
  }
  return y;
}

float Crv::bipolarize(float unipolar) const { return 2.f * unipolar - 1.f; }

float Crv::rectify(float bipolar) const { return bipolar * 0.5f + 0.5f; }

float Crv::wrap(float value, float min, float max) const {
  float range = max - min;
  float wrappedValue = fmod(value - min, range);
  if (wrappedValue < 0.f) {
    wrappedValue += range;
  } else if (wrappedValue == 0.f && value == max) {
    wrappedValue = range;
  }
  return wrappedValue + min;
}

float Crv::fold(float value, float min, float max) const {
  float range = max - min;
  float foldedValue = fmod((value - min), (2 * range));
  if (foldedValue < 0) foldedValue += 2 * range;
  return max - abs(foldedValue - range);
}

vector<float> Crv::floatArray(int numSamples, Component component) const {
  float step = 1.f / static_cast<float>(numSamples);
  vector<float> table(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    float x = i * step;
    table[i] = this->componentAt(component, x);
  }
  return table;
}

vector<float> Crv::floatArray(int numSamples) const {
  return floatArray(numSamples, Component::Y);
}

vector<glm::vec2> Crv::glv2Array(int numPoints, bool boxed, bool transformed,
                                 FloatOp samplingRateOp) const {
  vector<glm::vec2> points(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr) x = samplingRateOp(x);
    glm::vec3 v;
    if (boxed)
      v = wVector(x, transformed);
    else
      v = uVector(x, transformed);
    points[i] = glm::vec2(v.x, v.y);
  }
  return points;
}

vector<glm::vec3> Crv::glv3Array(int numPoints, bool boxed, bool transformed,
                                 FloatOp samplingRateOp) const {
  vector<glm::vec3> vectors(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr) x = samplingRateOp(x);
    if (boxed)
      vectors[i] = wVector(x, transformed);
    else
      vectors[i] = uVector(x, transformed);
    ;
  }
  return vectors;
}

vector<vector<float>> Crv::coordinateArray(int numPoints, bool boxed,
                                           bool transformed,
                                           FloatOp samplingRateOp) const {
  vector<glm::vec3> vectors =
      glv3Array(numPoints, boxed, transformed, samplingRateOp);
  vector<vector<float>> points(numPoints, vector<float>(3));
  for (int i = 0; i < numPoints; ++i) {
    points[i][0] = vectors[i].x;
    points[i][1] = vectors[i].y;
    points[i][2] = vectors[i].z;
  }
  return points;
}

vector<ofVec3f> Crv::ofv3Array(int numPoints, bool boxed, bool transformed,
                               FloatOp samplingRateOp) const {
  vector<ofVec3f> vectors(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr) x = samplingRateOp(x);
    glm::vec3 v;
    if (boxed)
      v = wVector(x, transformed);
    else
      v = uVector(x, transformed);
    vectors[i] = ofVec3f(v.x, v.y, v.z);
  }
  return vectors;
}

vector<ofVec2f> Crv::ofv2Array(int numPoints, bool boxed, bool transformed,
                               FloatOp samplingRateOp) const {
  vector<ofVec2f> vectors(numPoints);
  for (int i = 0; i < numPoints; ++i) {
    float x = static_cast<float>(i) / (numPoints - 1);
    if (samplingRateOp != nullptr) x = samplingRateOp(x);
    glm::vec3 v;
    if (boxed)
      v = wVector(x, transformed);
    else
      v = uVector(x, transformed);
    vectors[i] = ofVec2f(v.x, v.y);
  }
  return vectors;
}

ofPolyline Crv::polyline(int numPoints, bool boxed, bool transformed,
                         FloatOp samplingRateOp) const {
  vector<glm::vec3> vectors =
      glv3Array(numPoints, boxed, transformed, samplingRateOp);
  ofPolyline polyline;
  polyline.addVertices(vectors);
  return polyline;
}

glm::vec3 Crv::uVector(float pos, bool transformed) const {
  float x = xAt(pos);
  float y = yAt(pos);
  float z = zAt(pos);
  glm::vec3 v = glm::vec3(x, y, z);
  if (transformed) Utils::transform(v, UCENTER, scale, translation, rotation);
  bounded(v);
  return v;
}

glm::vec3 Crv::wVector(float pos, bool transformed) const {
  glm::vec3 v = uVector(pos, transformed);
  boxed(v);
  return v;
}

void Crv::bounded(glm::vec3& v) const {
  switch (bounding) {
    case Bounding::CLIPPING:
      glm::vec3(ofClamp(v.x, 0.f, 1.f), ofClamp(v.y, 0.f, 1.f),
                ofClamp(v.z, 0.f, 1.f));
      break;
    case Bounding::WRAPPING:
      wrapped(v);
      break;
    case Bounding::FOLDING:
      folded(v);
      break;
    default:
      break;
  }
}

void Crv::wrapped(glm::vec3& v) const {
  v.x = wrap(v.x, 0.f, 1.f);
  v.y = wrap(v.y, 0.f, 1.f);
  v.z = wrap(v.z, 0.f, 1.f);
}

void Crv::folded(glm::vec3& v) const {
  v.x = fold(v.x, 0.f, 1.f);
  v.y = fold(v.y, 0.f, 1.f);
  v.z = fold(v.z, 0.f, 1.f);
}

void Crv::boxed(glm::vec3& v) const { box.apply(v); }

vector<Edg> Crv::getWebEdgs(int numPoints, bool boxed, bool transformed,
                            int resolution) const {
  vector<glm::vec3> vectors = glv3Array(numPoints, boxed, transformed, nullptr);
  vector<Edg> edgs;
  for (int i = 0; i < vectors.size(); ++i) {
    for (int j = 0; j < vectors.size(); ++j) {
      if (i != j) {
        edgs.push_back(Edg(vectors[i], vectors[j], resolution));
      }
    }
  }
  return edgs;
}

vector<Edg> Crv::getWebEdgs(int numPoints, bool boxed, bool transformed) const {
  return getWebEdgs(numPoints, boxed, transformed, resolution);
}

}  // namespace ofxCrvs
