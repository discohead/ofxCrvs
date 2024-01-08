//
// Created by Jared McFarland on 1/7/24.
//

#pragma once

#ifndef OFXCRVSPTRN_H
#define OFXCRVSPTRN_H

#include "ofMain.h"
#include "ofxCrvsBufferedContainer.h"
#include "ofxCrvsCrv.h"

namespace ofxCrvs {

class Ptrn {
public:
  Ptrn()
      : trigXCache(trigXPattern()), trigYCache(trigYPattern()),
        trigZCache(trigZPattern()), valueXCache(valueXPattern()),
        valueYCache(valueYPattern()), valueZCache(valueZPattern()),
        vecCache(vectorPattern()) {}
  std::shared_ptr<Crv> crv;

  std::vector<float> trigPattern(int numStepsOverride = 0,
                                 float thresholdOverride = -1.f,
                                 Component component = Component::Y) const;

  std::vector<float> trigXPattern(int numStepsOverride = 0,
                                  float thresholdOverride = -1.f) const;

  std::vector<float> trigYPattern(int numStepsOverride = 0,
                                  float thresholdOverride = -1.f) const;

  std::vector<float> trigZPattern(int numStepsOverride = 0,
                                  float thresholdOverride = -1.f) const;

  std::vector<float> valuePattern(int numStepsOverride = 0,
                                  int numValuesOverride = 0,
                                  Component component = Component::Y) const;
  std::vector<float> valueXPattern(int numStepsOverride = 0,
                                   int numValuesOverride = 0) const;
  std::vector<float> valueYPattern(int numStepsOverride = 0,
                                   int numValuesOverride = 0) const;
  std::vector<float> valueZPattern(int numStepsOverride = 0,
                                   int numValuesOverride = 0) const;
  std::vector<glm::vec3> vectorPattern(int numStepsOverride = 0,
                                       bool transformed = true) const;

  void setTrigXReversed(bool reversed);
  void setTrigYReversed(bool reversed);
  void setTrigZReversed(bool reversed);
  void setValueXReversed(bool reversed);
  void setValueYReversed(bool reversed);
  void setValueZReversed(bool reversed);
  void setVecReversed(bool reversed);
  void setTrigXThreshold(float trigThreshold);
  void setTrigYThreshold(float trigThreshold);
  void setTrigZThreshold(float trigThreshold);
  void setSyncNext(bool sync);
  void setNumNextSteps(int numSteps);
  void setNumTrigXSteps(int numSteps);
  void setNumTrigYSteps(int numSteps);
  void setNumTrigZSteps(int numSteps);
  void setNumValueXSteps(int numSteps);
  void setNumValueYSteps(int numSteps);
  void setNumValueZSteps(int numSteps);
  void setNumValuesX(int numValues);
  void setNumValuesY(int numValues);
  void setNumValuesZ(int numValues);
  void setTransformedTrigX(bool transformed);
  void setTransformedTrigY(bool transformed);
  void setTransformedTrigZ(bool transformed);
  void setTransformedValueX(bool transformed);
  void setTransformedValueY(bool transformed);
  void setTransformedValueZ(bool transformed);
  void setAmpOffset(float ampOffset);
  void setRateOffset(float rateOffset);
  void setPhaseOffset(float phaseOffset);
  void setBiasOffset(float biasOffset);
  void setAmpModAmt(float ampModAmt);
  void setRateModAmt(float rateModAmt);
  void setPhaseModAmt(float phaseModAmt);
  void setBiasModAmt(float biasModAmt);
  void setOrigin(const glm::vec3 &origin);
  void setTranslation(const glm::vec3 &translation);
  void setScale(const glm::vec3 &scale);
  void setRotation(float rotation);
  void setBounding(Bounding bounding);
  void setCrv(const std::shared_ptr<Crv> &crv);

  std::array<std::array<float, 3>, 2> next();
  float nextTrigX();
  float nextTrigY();
  float nextTrigZ();
  float nextValueX();
  float nextValueY();
  float nextValueZ();
  glm::vec3 nextVec();

  float trigXAt(float pos) const;
  float trigYAt(float pos) const;
  float trigZAt(float pos) const;
  float valueXAt(float pos) const;
  float valueYAt(float pos) const;
  float valueZAt(float pos) const;
  glm::vec3 vecAt(float pos) const;

  float trigXAt(int index) const;
  float trigYAt(int index) const;
  float trigZAt(int index) const;
  float valueXAt(int index) const;
  float valueYAt(int index) const;
  float valueZAt(int index) const;
  glm::vec3 vecAt(int index) const;

  void sync();
  void resetNext();
  void resetTrigX();
  void resetTrigY();
  void resetTrigZ();
  void resetValueX();
  void resetValueY();
  void resetValueZ();
  void resetVec();

  void updateCache();
  void updateTrigXCache();
  void updateTrigYCache();
  void updateTrigZCache();
  void updateValueXCache();
  void updateValueYCache();
  void updateValueZCache();
  void updateVecCache();

  size_t maxSize() const;

private:
  std::atomic<bool> syncNext{true};
  std::atomic<bool> transformedTrigX{false};
  std::atomic<bool> transformedTrigY{false};
  std::atomic<bool> transformedTrigZ{false};
  std::atomic<bool> transformedValueX{false};
  std::atomic<bool> transformedValueY{false};
  std::atomic<bool> transformedValueZ{false};

  std::atomic<float> trigXThreshold{0.5f};
  std::atomic<float> trigYThreshold{0.5f};
  std::atomic<float> trigZThreshold{0.5f};

  std::atomic<int> numNextSteps{16};
  std::atomic<int> numTrigXSteps{16};
  std::atomic<int> numTrigYSteps{16};
  std::atomic<int> numTrigZSteps{16};
  std::atomic<int> numValueXSteps{16};
  std::atomic<int> numValueYSteps{16};
  std::atomic<int> numValueZSteps{16};
  std::atomic<int> numVecSteps{16};

  std::atomic<int> numValuesX{0};
  std::atomic<int> numValuesY{0};
  std::atomic<int> numValuesZ{0};

  std::atomic<bool> trigXReversed{false};
  std::atomic<bool> trigYReversed{false};
  std::atomic<bool> trigZReversed{false};
  std::atomic<bool> valueXReversed{false};
  std::atomic<bool> valueYReversed{false};
  std::atomic<bool> valueZReversed{false};
  std::atomic<bool> vecReversed{false};

  std::atomic<int> currentNextIndex{0};
  std::atomic<int> currentTrigXIndex{0};
  std::atomic<int> currentTrigYIndex{0};
  std::atomic<int> currentTrigZIndex{0};
  std::atomic<int> currentValueXIndex{0};
  std::atomic<int> currentValueYIndex{0};
  std::atomic<int> currentValueZIndex{0};
  std::atomic<int> currentVecIndex{0};

  BufferedContainer<std::vector<float>> trigXCache;
  BufferedContainer<std::vector<float>> trigYCache;
  BufferedContainer<std::vector<float>> trigZCache;
  BufferedContainer<std::vector<float>> valueXCache;
  BufferedContainer<std::vector<float>> valueYCache;
  BufferedContainer<std::vector<float>> valueZCache;
  BufferedContainer<std::vector<glm::vec3>> vecCache;

  static float quantize(float y, int quantization);
};

} // namespace ofxCrvs

#endif // OFXCRVSPTRN_H
