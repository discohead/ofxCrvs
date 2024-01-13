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
      : crv(Crv::create()), trigXCache(trigsX()), trigYCache(trigsY()),
        trigZCache(trigsZ()), valueXCache(valuesX()), valueYCache(valuesY()),
        valueZCache(valuesZ()), vecCache(vecs()) {}
  std::shared_ptr<Crv> crv;

  std::vector<float> trigs(int numStepsOverride = 0,
                           float thresholdOverride = -1.f,
                           Component component = Component::Y) const;

  std::vector<float> trigsX(int numStepsOverride = 0,
                            float thresholdOverride = -1.f) const;

  std::vector<float> trigsY(int numStepsOverride = 0,
                            float thresholdOverride = -1.f) const;

  std::vector<float> trigsZ(int numStepsOverride = 0,
                            float thresholdOverride = -1.f) const;
  // TODO: trig patterns using logical operators i.e trigXANDTrigYPattern()
  // maybe overload operator&& and operator|| for Ptrn?
  std::vector<float> values(int numStepsOverride = 0, int numValuesOverride = 0,
                            Component component = Component::Y) const;
  std::vector<float> valuesX(int numStepsOverride = 0,
                             int numValuesOverride = 0) const;
  std::vector<float> valuesY(int numStepsOverride = 0,
                             int numValuesOverride = 0) const;
  std::vector<float> valuesZ(int numStepsOverride = 0,
                             int numValuesOverride = 0) const;
  std::vector<glm::vec3> vecs(int numStepsOverride = 0,
                              bool transformed = true) const;

  bool getTrigXTransformed() const;
  bool getTrigYTransformed() const;
  bool getTrigZTransformed() const;

  bool getValueXTransformed() const;
  bool getValueYTransformed() const;
  bool getValueZTransformed() const;

  bool getTrigXInverted() const;
  bool getTrigYInverted() const;
  bool getTrigZInverted() const;

  bool getTrigXReversed() const;
  bool getTrigYReversed() const;
  bool getTrigZReversed() const;

  bool getValueXReversed() const;
  bool getValueYReversed() const;
  bool getValueZReversed() const;

  void setTrigXInverted(bool inverted);
  void setTrigYInverted(bool inverted);
  void setTrigZInverted(bool inverted);

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
  void setNumVecSteps(int numSteps);

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

  std::array<std::array<float, 2>, 3> next();
  float nextTrig(Component component = Component::Y);
  float nextTrigX();
  float nextTrigY();
  float nextTrigZ();
  float nextValue(Component component = Component::Y);
  float nextValueX();
  float nextValueY();
  float nextValueZ();
  glm::vec3 nextVec();

  float trigAt(float pos, Component component = Component::Y) const;
  float trigXAt(float pos) const;
  float trigYAt(float pos) const;
  float trigZAt(float pos) const;
  float valueAt(float pos, Component component = Component::Y) const;
  float valueXAt(float pos) const;
  float valueYAt(float pos) const;
  float valueZAt(float pos) const;
  glm::vec3 vecAt(float pos) const;

  float trigAt(int index, Component component = Component::Y) const;
  float trigXAt(int index) const;
  float trigYAt(int index) const;
  float trigZAt(int index) const;
  float valueAt(int index, Component component = Component::Y) const;
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

private:
  std::atomic<bool> syncNext{true};

  std::atomic<float> ampOffset{1.f};
  std::atomic<float> rateOffset{1.f};
  std::atomic<float> phaseOffset{0.f};
  std::atomic<float> biasOffset{0.f};

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

  std::atomic<bool> trigXInverted{false};
  std::atomic<bool> trigYInverted{false};
  std::atomic<bool> trigZInverted{false};

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
