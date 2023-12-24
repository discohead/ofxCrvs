
#include "ofxCrvsCrv.h"

#include <algorithm>

#include "ofxCrvsEdg.hpp"
#include "ofxCrvsUtils.hpp"

namespace ofxCrvs {

float Crv::calculate(float pos) { return quantize(op(pos)); }

float Crv::apply(float pos) { return calculate(pos); }

float Crv::ampBias(float value, float pos) {
  float ampFactor = ampOffset;
  if (amp != nullptr) ampFactor *= amp->yAt(pos);
  ampFactor = ampFactor / 2.f;
  value = value * ampFactor + ampFactor;
  if (bias != nullptr) value += bias->yAt(pos);
  return value + biasOffset;
}

float Crv::calcPos(float pos) {
  pos = abs(pos);
  pos *= rateOffset;
  if (pos > 1.f) pos = fmod(pos, 1.f);
  if (rate != nullptr) pos *= rate->yAt(pos);
  if (phase != nullptr) pos += phase->yAt(pos);
  pos += phaseOffset;
  if (pos > 1.f) pos = fmod(pos, 1.f);
  return pos;
}

float Crv::componentAt(Component component, float pos) {
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

float Crv::xAt(float pos) { return componentAt(Component::X, pos); }

float Crv::yAt(float pos) { return componentAt(Component::Y, pos); }

float Crv::zAt(float pos) { return componentAt(Component::Z, pos); }

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
  if (foldedValue < 0) foldedValue += 2 * range;
  return max - abs(foldedValue - range);
}

vector<float> Crv::floatArray(int numSamples, Component component) {
  float step = 1.f / static_cast<float>(numSamples);
  vector<float> table(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    float x = i * step;
    table[i] = this->componentAt(component, x);
  }
  return table;
}

vector<float> Crv::floatArray(int numSamples) {
  return floatArray(numSamples, Component::Y);
}

vector<glm::vec2> Crv::glv2Array(int numPoints, bool boxed, bool transformed,
                                 FloatOp samplingRateOp) {
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
                                 FloatOp samplingRateOp) {
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
                                           FloatOp samplingRateOp) {
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
                               FloatOp samplingRateOp) {
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
                               FloatOp samplingRateOp) {
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
                         FloatOp samplingRateOp) {
  vector<glm::vec3> vectors =
      glv3Array(numPoints, boxed, transformed, samplingRateOp);
  ofPolyline polyline;
  polyline.addVertices(vectors);
  return polyline;
}

glm::vec3 Crv::uVector(float pos, bool transformed) {
  float x = xAt(pos);
  float y = yAt(pos);
  float z = zAt(pos);
  glm::vec3 v = glm::vec3(x, y, z);
  if (transformed) v = transform(v);
  v += origin;
  return bounded(v);
}

glm::vec3 Crv::wVector(float pos, bool transformed) {
  glm::vec3 v = uVector(pos, transformed);
  return boxed(v);
}

glm::vec3 Crv::transform(glm::vec3 v) {
  return Utils::transform(v, Crv::uCenter, scale, translation, rotation);
}

glm::vec3 Crv::bounded(glm::vec3 v) {
  switch (bounding) {
    case Bounding::CLIPPING:
      return Utils::clipped(v, box);
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

glm::vec3 Crv::wrapped(glm::vec3 v) {
  v.x = wrap(v.x, 0.f, 1.f);
  v.y = wrap(v.y, 0.f, 1.f);
  return v;
}

glm::vec3 Crv::folded(glm::vec3 v) {
  v.x = fold(v.x, 0.f, 1.f);
  v.y = fold(v.y, 0.f, 1.f);
  return v;
}

glm::vec3 Crv::boxed(glm::vec3 v) { return box.apply(v); }

vector<Edg> Crv::getWebEdgs(int numPoints, bool boxed, bool transformed,
                            int resolution) {
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

vector<Edg> Crv::getWebEdgs(int numPoints, bool boxed, bool transformed) {
  return getWebEdgs(numPoints, boxed, transformed, resolution);
}

}  // namespace ofxCrvs
