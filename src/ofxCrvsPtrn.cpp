//
// Created by Jared McFarland on 1/7/24.
//

#include "ofxCrvsPtrn.h"

namespace ofxCrvs {

std::vector<float> Ptrn::trigPattern(const int numStepsOverride,
                                     const float thresholdOverride,
                                     const Component component) const {
  int numSteps = 0;
  float threshold = 0.f;
  bool transformed = false;
  switch (component) {
  case Component::X:
    numSteps = numStepsOverride > 0 ? numStepsOverride : numTrigXSteps.load();
    threshold =
        thresholdOverride >= 0.f ? thresholdOverride : trigXThreshold.load();
    transformed = transformedValueX.load();
    break;
  case Component::Z:
    numSteps = numStepsOverride > 0 ? numStepsOverride : numTrigZSteps.load();
    threshold =
        thresholdOverride >= 0.f ? thresholdOverride : trigZThreshold.load();
    transformed = transformedValueZ.load();
    break;
  default:
    numSteps = numStepsOverride > 0 ? numStepsOverride : numTrigYSteps.load();
    threshold =
        thresholdOverride >= 0.f ? thresholdOverride : trigYThreshold.load();
    transformed = transformedValueY.load();
    break;
  }
  auto trigs = std::vector<float>(numSteps);
  if (transformed) {
    std::vector<glm::vec3> v = vectorPattern(numSteps, true);
    for (int i = 0; i < numSteps; ++i) {
      trigs[i] = v[i][static_cast<int>(component)];
    }
  } else {
    trigs = crv->floatArray(numSteps, component);
  }
  for (float &f : trigs) {
    f = f > threshold ? 1.0 : 0.0;
  }
  return trigs;
}

std::vector<float> Ptrn::trigXPattern(const int numStepsOverride,
                                      const float thresholdOverride) const {
  return trigPattern(numStepsOverride, thresholdOverride, Component::X);
}

std::vector<float> Ptrn::trigYPattern(const int numStepsOverride,
                                      const float thresholdOverride) const {
  return trigPattern(numStepsOverride, thresholdOverride, Component::Y);
}

std::vector<float> Ptrn::trigZPattern(const int numStepsOverride,
                                      const float thresholdOverride) const {
  return trigPattern(numStepsOverride, thresholdOverride, Component::Z);
}

std::vector<float> Ptrn::valuePattern(const int numStepsOverride,
                                      const int numValuesOverride,
                                      const Component component) const {
  int numSteps = 0;
  int numValues = 0;
  bool transformed = false;
  switch (component) {
  case Component::X:
    numSteps = numStepsOverride > 0 ? numStepsOverride : numValueXSteps.load();
    numValues = numValuesOverride > 1 ? numValuesOverride : numValuesX.load();
    transformed = transformedValueX.load();
    break;
  case Component::Z:
    numSteps = numStepsOverride > 0 ? numStepsOverride : numValueZSteps.load();
    numValues = numValuesOverride > 1 ? numValuesOverride : numValuesZ.load();
    transformed = transformedValueZ.load();
    break;
  default:
    numSteps = numStepsOverride > 0 ? numStepsOverride : numValueYSteps.load();
    numValues = numValuesOverride > 1 ? numValuesOverride : numValuesY.load();
    transformed = transformedValueY.load();
    break;
  }
  auto p = std::vector<float>(numSteps);
  if (transformed) {
    std::vector<glm::vec3> v = vectorPattern(numSteps, true);
    for (int i = 0; i < numSteps; ++i) {
      p[i] = v[i][static_cast<int>(component)];
    }
  } else {
    p = crv->floatArray(numSteps, component);
  }
  for (float &f : p) {
    f = quantize(f, numValues);
  }
  return p;
}

std::vector<float> Ptrn::valueXPattern(const int numStepsOverride,
                                       const int numValuesOverride) const {
  return valuePattern(numStepsOverride, numValuesOverride, Component::X);
}

std::vector<float> Ptrn::valueYPattern(const int numStepsOverride,
                                       const int numValuesOverride) const {
  return valuePattern(numStepsOverride, numValuesOverride, Component::Y);
}

std::vector<float> Ptrn::valueZPattern(const int numStepsOverride,
                                       const int numValuesOverride) const {
  return valuePattern(numStepsOverride, numValuesOverride, Component::Z);
}

std::vector<glm::vec3> Ptrn::vectorPattern(const int numStepsOverride,
                                           const bool transformed) const {
  const int numSteps =
      numStepsOverride > 0 ? numStepsOverride : numValueYSteps.load();
  return crv->glv3Array(numSteps, false, transformed);
}

float Ptrn::quantize(const float y, const int quantization) {
  if (quantization > 1) {
    const float levelSize = 1.f / (quantization - 1);
    const int quantizedLevel = round(y / levelSize);
    return quantizedLevel * levelSize;
  }
  return y;
}

void Ptrn::setTrigXReversed(const bool reversed) {
  trigXReversed.store(reversed);
}

void Ptrn::setTrigYReversed(const bool reversed) {
  trigYReversed.store(reversed);
}

void Ptrn::setTrigZReversed(const bool reversed) {
  trigZReversed.store(reversed);
}

void Ptrn::setValueXReversed(const bool reversed) {
  valueXReversed.store(reversed);
}

void Ptrn::setValueYReversed(const bool reversed) {
  valueYReversed.store(reversed);
}

void Ptrn::setValueZReversed(const bool reversed) {
  valueZReversed.store(reversed);
}

void Ptrn::setVecReversed(const bool reversed) { vecReversed.store(reversed); }

void Ptrn::setTrigXThreshold(const float trigThreshold) {
  trigXThreshold.store(trigThreshold);
  updateTrigXCache();
}

void Ptrn::setTrigYThreshold(const float trigThreshold) {
  trigYThreshold.store(trigThreshold);
  updateTrigYCache();
}

void Ptrn::setTrigZThreshold(const float trigThreshold) {
  trigZThreshold.store(trigThreshold);
  updateTrigZCache();
}

void Ptrn::setSyncNext(const bool sync) { syncNext.store(sync); }

void Ptrn::setNumNextSteps(const int numSteps) { numNextSteps.store(numSteps); }

void Ptrn::setNumTrigXSteps(const int numSteps) {
  numTrigXSteps.store(numSteps);
  updateTrigXCache();
}

void Ptrn::setNumTrigYSteps(const int numSteps) {
  numTrigYSteps.store(numSteps);
  updateTrigYCache();
}

void Ptrn::setNumTrigZSteps(const int numSteps) {
  numTrigZSteps.store(numSteps);
  updateTrigZCache();
}

void Ptrn::setNumValueXSteps(const int numSteps) {
  numValueXSteps.store(numSteps);
  updateValueXCache();
}

void Ptrn::setNumValueYSteps(const int numSteps) {
  numValueYSteps.store(numSteps);
  updateValueYCache();
}

void Ptrn::setNumValueZSteps(const int numSteps) {
  numValueZSteps.store(numSteps);
  updateValueZCache();
}

void Ptrn::setNumValuesX(const int numValues) {
  numValuesX.store(numValues);
  updateValueXCache();
}

void Ptrn::setNumValuesY(const int numValues) {
  numValuesY.store(numValues);
  updateValueYCache();
}

void Ptrn::setNumValuesZ(const int numValues) {
  numValuesZ.store(numValues);
  updateValueZCache();
}

void Ptrn::setTransformedTrigX(const bool transformed) {
  transformedTrigX.store(transformed);
  updateTrigXCache();
}

void Ptrn::setTransformedTrigY(const bool transformed) {
  transformedTrigY.store(transformed);
  updateTrigYCache();
}

void Ptrn::setTransformedTrigZ(const bool transformed) {
  transformedTrigZ.store(transformed);
  updateTrigZCache();
}

void Ptrn::setTransformedValueX(const bool transformed) {
  transformedValueX.store(transformed);
  updateValueXCache();
}

void Ptrn::setTransformedValueY(const bool transformed) {
  transformedValueY.store(transformed);
  updateValueYCache();
}

void Ptrn::setTransformedValueZ(const bool transformed) {
  transformedValueZ.store(transformed);
  updateValueZCache();
}

void Ptrn::setAmpOffset(const float ampOffset) {
  crv->ampOffset = ampOffset;
  updateCache();
}

void Ptrn::setRateOffset(const float rateOffset) {
  crv->rateOffset = rateOffset;
  updateCache();
}

void Ptrn::setPhaseOffset(const float phaseOffset) {
  crv->phaseOffset = phaseOffset;
  updateCache();
}

void Ptrn::setBiasOffset(const float biasOffset) {
  crv->biasOffset = biasOffset;
  updateCache();
}

void Ptrn::setAmpModAmt(const float ampModAmt) {
  crv->ampModAmt = ampModAmt;
  updateCache();
}

void Ptrn::setRateModAmt(const float rateModAmt) {
  crv->rateModAmt = rateModAmt;
  updateCache();
}

void Ptrn::setPhaseModAmt(const float phaseModAmt) {
  crv->phaseModAmt = phaseModAmt;
  updateCache();
}

void Ptrn::setBiasModAmt(const float biasModAmt) {
  crv->biasModAmt = biasModAmt;
  updateCache();
}

void Ptrn::setOrigin(const glm::vec3 &origin) {
  crv->origin = origin;
  updateCache();
}

void Ptrn::setTranslation(const glm::vec3 &translation) {
  crv->translation = translation;
  updateCache();
}

void Ptrn::setScale(const glm::vec3 &scale) {
  crv->scale = scale;
  updateCache();
}

void Ptrn::setRotation(const float rotation) {
  crv->rotation = rotation;
  updateCache();
}

void Ptrn::setBounding(const Bounding bounding) {
  crv->bounding = bounding;
  updateCache();
}

void Ptrn::setCrv(const std::shared_ptr<Crv> &crv) {
  this->crv = crv;
  updateCache();
}

void Ptrn::sync() {
  resetTrigX();
  resetTrigY();
  resetTrigZ();
  resetValueX();
  resetValueY();
  resetValueZ();
  resetVec();
}

void Ptrn::resetNext() { currentNextIndex.store(0); }
void Ptrn::resetTrigX() { currentTrigXIndex.store(0); }
void Ptrn::resetTrigY() { currentTrigYIndex.store(0); }
void Ptrn::resetTrigZ() { currentTrigZIndex.store(0); }
void Ptrn::resetValueX() { currentValueXIndex.store(0); }
void Ptrn::resetValueY() { currentValueYIndex.store(0); }
void Ptrn::resetValueZ() { currentValueZIndex.store(0); }
void Ptrn::resetVec() { currentVecIndex.store(0); }

void Ptrn::updateCache() {
  updateTrigXCache();
  updateTrigYCache();
  updateTrigZCache();
  updateValueXCache();
  updateValueYCache();
  updateValueZCache();
  updateVecCache();
}

void Ptrn::updateTrigXCache() { trigXCache.set(trigXPattern()); }

void Ptrn::updateTrigYCache() { trigYCache.set(trigYPattern()); }

void Ptrn::updateTrigZCache() { trigZCache.set(trigZPattern()); }

void Ptrn::updateValueXCache() { valueXCache.set(valueXPattern()); }

void Ptrn::updateValueYCache() { valueYCache.set(valueYPattern()); }

void Ptrn::updateValueZCache() { valueZCache.set(valueZPattern()); }

void Ptrn::updateVecCache() { vecCache.set(vectorPattern()); }

float Ptrn::nextTrigX() {
  int idx = currentTrigXIndex.load();
  const float trig = trigXCache[idx];
  if (trigXReversed.load())
    idx--;
  else
    idx++;
  if (idx >= trigXCache.size())
    idx = 0;
  else if (idx < 0)
    idx = trigXCache.size() - 1;
  currentTrigXIndex.store(idx);
  return trig;
}

float Ptrn::nextTrigY() {
  int idx = currentTrigYIndex.load();
  const float trig = trigYCache[idx];
  if (trigYReversed.load())
    idx--;
  else
    idx++;
  if (idx >= trigYCache.size())
    idx = 0;
  else if (idx < 0)
    idx = trigYCache.size() - 1;
  currentTrigYIndex.store(idx);
  return trig;
}

float Ptrn::nextTrigZ() {
  int idx = currentTrigZIndex.load();
  const float trig = trigZCache[idx];
  if (trigZReversed.load())
    idx--;
  else
    idx++;
  if (idx >= trigZCache.size())
    idx = 0;
  else if (idx < 0)
    idx = trigZCache.size() - 1;
  currentTrigZIndex.store(idx);
  return trig;
}

float Ptrn::nextValueX() {
  int idx = currentValueXIndex.load();
  const float value = valueXCache[idx];
  if (valueXReversed.load())
    idx--;
  else
    idx++;
  if (idx >= valueXCache.size())
    idx = 0;
  else if (idx < 0)
    idx = valueXCache.size() - 1;
  currentValueXIndex.store(idx);
  return value;
}

float Ptrn::nextValueY() {
  int idx = currentValueYIndex.load();
  const float value = valueYCache[idx];
  if (valueYReversed.load())
    idx--;
  else
    idx++;
  if (idx >= valueYCache.size())
    idx = 0;
  else if (idx < 0)
    idx = valueYCache.size() - 1;
  currentValueYIndex.store(idx);
  return value;
}

float Ptrn::nextValueZ() {
  int idx = currentValueZIndex.load();
  const float value = valueZCache[idx];
  if (valueZReversed.load())
    idx--;
  else
    idx++;
  if (idx >= valueZCache.size())
    idx = 0;
  else if (idx < 0)
    idx = valueZCache.size() - 1;
  currentValueZIndex.store(idx);
  return value;
}

glm::vec3 Ptrn::nextVec() {
  int idx = currentVecIndex.load();
  const glm::vec3 vec = vecCache[idx];
  if (vecReversed.load())
    idx--;
  else
    idx++;
  if (idx >= vecCache.size())
    idx = 0;
  else if (idx < 0)
    idx = vecCache.size() - 1;
  currentVecIndex.store(idx);
  return vec;
}

std::array<std::array<float, 3>, 2> Ptrn::next() {
  int idx = currentNextIndex.load();
  if (idx >= numNextSteps.load()) {
    if (syncNext.load())
      sync();
    idx = 0;
  } else {
    idx++;
  }
  currentNextIndex.store(idx);
  const std::array<float, 3> trigs = {nextTrigX(), nextTrigY(), nextTrigZ()};
  const std::array<float, 3> values = {nextValueX(), nextValueY(),
                                       nextValueZ()};
  const std::array<std::array<float, 3>, 2> trigsAndValues = {trigs, values};
  return trigsAndValues;
}

float Ptrn::trigXAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * trigXCache.size());
  return trigXCache[idx];
}

float Ptrn::trigYAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * trigYCache.size());
  return trigYCache[idx];
}

float Ptrn::trigZAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * trigZCache.size());
  return trigZCache[idx];
}

float Ptrn::valueXAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * valueXCache.size());
  return valueXCache[idx];
}

float Ptrn::valueYAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * valueYCache.size());
  return valueYCache[idx];
}

float Ptrn::valueZAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * valueZCache.size());
  return valueZCache[idx];
}

glm::vec3 Ptrn::vecAt(const float pos) const {
  const int idx = static_cast<int>(fmod(pos, 1.f) * vecCache.size());
  return vecCache[idx];
}

float Ptrn::trigXAt(const int index) const { return trigXCache[index]; }

float Ptrn::trigYAt(const int index) const { return trigYCache[index]; }

float Ptrn::trigZAt(const int index) const { return trigZCache[index]; }

float Ptrn::valueXAt(const int index) const { return valueXCache[index]; }

float Ptrn::valueYAt(const int index) const { return valueYCache[index]; }

float Ptrn::valueZAt(const int index) const { return valueZCache[index]; }

} // namespace ofxCrvs