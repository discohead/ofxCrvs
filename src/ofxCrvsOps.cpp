
#include "ofxCrvsOps.h"

namespace ofxCrvs {

float Ops::pos2Rad(float pos) {
  pos = ofClamp(pos, 0.f, 1.f);
  float degrees = pos * 360.f;
  return ofDegToRad(degrees);
  return pos;
}

FloatOp Ops::bipolarize(FloatOp unipolarOp) const {
  return [unipolarOp](float pos) { return 2.f * unipolarOp(pos) - 1.f; };
}

FloatOp Ops::rectify(FloatOp bipolarOp) const {
  return [bipolarOp](float pos) {
    float bipolarValue = bipolarOp(pos);
    return bipolarValue * 0.5f + 0.5f;
  };
}

FloatOp Ops::c(float value) const {
  return [value](float /*pos*/) { return value; };
}

FloatOp Ops::phasor() const {
  return [](float pos) { return pos; };
}

FloatOp Ops::saw() const {
  return [](float pos) { return 1.f - pos; };
}

FloatOp Ops::tri(FloatOp s) const {
  return [s](float pos) {
    float sValue = 0.5f;
    if (s) sValue = s(pos);
    return pos < sValue ? pos / sValue
                        : 1.f - ((pos - sValue) / (1.f - sValue));
  };
}

FloatOp Ops::tri() const { return tri(nullptr); }

FloatOp Ops::tri(float s) const { return tri(c(s)); }

FloatOp Ops::sine(FloatOp fb) const {
  return [fb](float pos) {
    if (fb) {
      float fbScale = fb(pos);
      pos = pos + fbScale * (std::sin(pos2Rad(pos)) * 0.5f) + 0.5f;
    }
    return (std::sin(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::sine() const { return sine(nullptr); }

FloatOp Ops::sine(float fb) const { return sine(c(fb)); }

FloatOp Ops::asin() const {
  return [](float pos) {
    pos = pos * 2.f - 1.f;
    return (std::asin(pos) + glm::half_pi<float>()) / glm::pi<float>();
  };
}

FloatOp Ops::cos(FloatOp fb) const {
  return [fb](float pos) {
    if (fb) {
      float fbScale = fb(pos);
      pos = pos + fbScale * (std::cos(pos2Rad(pos)) * 0.5f) + 0.5f;
    }
    return (std::cos(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::cos() const { return cos(nullptr); }

FloatOp Ops::cos(float fb) const { return cos(c(fb)); }

FloatOp Ops::acos() const {
  return [](float pos) {
    pos = pos * 2.f - 1.f;
    return std::acos(pos) / glm::pi<float>();
  };
}

FloatOp Ops::tan(FloatOp fb) const {
  return [fb](float pos) {
    if (fb) {
      float fbScale = fb(pos);
      pos = pos + fbScale * (std::tan(pos2Rad(pos)) * 0.5f) + 0.5f;
    }
    return (std::tan(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::tan() const { return tan(nullptr); }

FloatOp Ops::tan(float fb) const { return tan(c(fb)); }

FloatOp Ops::pulse(FloatOp w) const {
  return [w](float pos) {
    float wValue = 0.5f;
    if (w) wValue = w(pos);
    return pos < wValue ? 0.f : 1.f;
  };
}

FloatOp Ops::pulse() const { return pulse(nullptr); }

FloatOp Ops::pulse(float w) const { return pulse(c(w)); }

FloatOp Ops::square() const { return pulse(); }

FloatOp Ops::easeIn(FloatOp e) const {
  return [e](float pos) {
    float eValue = 2.f;
    if (e) eValue = e(pos);
    return std::pow(pos, eValue);
  };
}

FloatOp Ops::easeIn() const { return easeIn(nullptr); }

FloatOp Ops::easeIn(float e) const { return easeIn(c(e)); }

FloatOp Ops::easeOut(FloatOp e) const {
  return [e](float pos) {
    float eValue = 3.f;
    if (e) eValue = e(pos);
    return 1.f - std::pow((1.f - pos), eValue);
  };
}

FloatOp Ops::easeOut() const { return easeOut(nullptr); }

FloatOp Ops::easeOut(float e) const { return easeOut(c(e)); }

FloatOp Ops::easeInOut(FloatOp e) const {
  return [e](float pos) {
    float value = pos * 2.f;
    float eValue = 3.f;
    if (e) eValue = e(pos);
    if (value > 1.f) {
      return 0.5f * std::pow(value, eValue);
    } else {
      return 0.5f * (2.f - std::pow((2.f - value), eValue));
    }
  };
}

FloatOp Ops::easeInOut() const { return easeInOut(nullptr); }

FloatOp Ops::easeInOut(float e) const { return easeInOut(c(e)); }

FloatOp Ops::easeOutIn(FloatOp e) const {
  return [e](float pos) {
    float value = pos * 2.f;
    float eValue = 3.f;
    if (e) eValue = e(pos);
    if (value < 1.f) {
      return (1.f - std::pow((1.f - value), eValue) * 0.5f) - 0.5f;
    } else {
      value = value - 1.f;
      return (std::pow(value, eValue) * 0.5f) + 0.5f;
    }
  };
}

FloatOp Ops::easeOutIn() const { return easeOutIn(nullptr); }

FloatOp Ops::easeOutIn(float e) const { return easeOutIn(c(e)); }

FloatOp Ops::gaussian(FloatOp lo, FloatOp hi) const {
  return [lo, hi](float pos) {
    float g = ofRandomGaussian(0.f, 1.f);
    if (g < -1.f)
      g = fmod(g, -1.f);
    else if (g > 1.f)
      g = fmod(g, 1.f);
    g = (g + 1.f) * 0.5f;
    float loVal = lo ? lo(pos) : 0.f;
    float hiVal = hi ? hi(pos) : 1.f;
    return loVal + (g * (hiVal - loVal));
  };
}

FloatOp Ops::random(FloatOp lo, FloatOp hi, FloatOp mode) const {
  return [lo, hi, mode, this](float pos) {
    float loVal = lo ? lo(pos) : 0.f;
    float hiVal = hi ? hi(pos) : 1.f;
    float modeVal = mode ? mode(pos) : 0.5f;
    if (mode) {
      modeVal = mode(pos);
      return triDist(loVal, hiVal, modeVal);
    } else {
      return loVal + ofRandom(hiVal - loVal);
    }
  };
}

FloatOp Ops::wavetable() const {
  return [this](float pos) {
    int t = static_cast<int>(ofMap(pos, 0.f, 1.f, 0.f, table.size()));
    float samp = table[t];
    return (samp + 1.f) * 0.5f;
  };
}

FloatOp Ops::perlin(FloatOp x, FloatOp y, FloatOp z, FloatOp falloff,
                    FloatOp octaves) const {
  return [x, y, z, falloff, octaves, this](float pos) {
    int lod = 1;
    float fof = 1.f;
    if (falloff) fof = falloff(pos);
    if (octaves) lod = octaves(pos);
    float xVal = x(pos);
    if (!y) return pNoise(xVal, fof, lod);
    float yVal = y(pos);
    if (!z) return pNoise(xVal, yVal, fof, lod);
    float zVal = z(pos);
    return pNoise(xVal, yVal, zVal, fof, lod);
  };
}

FloatOp Ops::mult(FloatOp op, float scalar) const {
  return [op, scalar](float pos) { return op(pos) * scalar; };
}

FloatOp Ops::bias(FloatOp op, float offset) const {
  return [op, offset](float pos) { return op(pos) + offset; };
}

FloatOp Ops::bias(FloatOp op, FloatOp offset) const {
  return [op, offset](float pos) { return op(pos) + offset(pos); };
}

FloatOp Ops::phase(FloatOp op, float phaseOffset) const {
  return [op, phaseOffset](float pos) {
    pos = pos + phaseOffset;
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::phase(FloatOp op, FloatOp phaseOffset) const {
  return [op, phaseOffset](float pos) {
    pos = pos + phaseOffset(pos);
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::rate(FloatOp op, float rateOffset) const {
  return [op, rateOffset](float pos) {
    pos = pos * rateOffset;
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::rate(FloatOp op, FloatOp rateOffset) const {
  return [op, rateOffset](float pos) {
    pos = pos * rateOffset(pos);
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::ring(FloatOp opA, FloatOp opB) const {
  return [opA, opB](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return a * b;
  };
}

FloatOp Ops::fold(FloatOp op, FloatOp threshold) const {
  return [op, threshold](float pos) {
    float val = op(pos);
    float thresh = threshold(pos);
    while (val > thresh) {
      val = thresh - (val - thresh);
    }
    return val;
  };
}

FloatOp Ops::fold(FloatOp op, float threshold) const {
  return [op, threshold](float pos) {
    float val = op(pos);
    while (val > threshold) {
      val = threshold - (val - threshold);
    }
    return val;
  };
}

FloatOp Ops::fold(FloatOp op) const {
  return [op](float pos) {
    float val = op(pos);
    while (val > 1.f) {
      val = 1.f - (val - 1.f);
    }
    return val;
  };
}

FloatOp Ops::lpf(FloatOp inputOp, int windowSize) const {
  return [inputOp, windowSize](float pos) {
    float sum = 0.f;
    for (int i = 0; i < windowSize; ++i) {
      float offsetPos = pos - (static_cast<float>(i) / windowSize);
      offsetPos = std::max(0.f, std::min(1.f, offsetPos));
      sum += inputOp(offsetPos);
    }
    return sum / windowSize;
  };
}

FloatOp Ops::mix(vector<FloatOp> ops) const {
  return [ops](float pos) {
    float sum = 0.f;
    for (auto op : ops) {
      sum += op(pos);
    }
    return sum / ops.size();
  };
}

FloatOp Ops::mix(vector<FloatOp> ops, vector<float> levels) const {
  return [ops, levels](float pos) {
    float sum = 0.f;
    for (int i = 0; i < ops.size(); ++i) {
      sum += ops[i](pos) * levels[i];
    }
    return sum / ops.size();
  };
}

FloatOp Ops::sum(vector<FloatOp> ops) const {
  return [ops](float pos) {
    float sum = 0.f;
    for (auto op : ops) {
      sum += op(pos);
    }
    return sum;
  };
}

FloatOp Ops::product(vector<FloatOp> ops) const {
  return [ops](float pos) {
    float product = 1.f;
    for (auto op : ops) {
      product *= op(pos);
    }
    return product;
  };
}

FloatOp Ops::min(vector<FloatOp> ops) const {
  return [ops](float pos) {
    float min = std::numeric_limits<float>::max();
    for (auto op : ops) {
      float val = op(pos);
      if (val < min) min = val;
    }
    return min;
  };
}

FloatOp Ops::max(vector<FloatOp> ops) const {
  return [ops](float pos) {
    float max = std::numeric_limits<float>::min();
    for (auto op : ops) {
      float val = op(pos);
      if (val > max) max = val;
    }
    return max;
  };
}

FloatOp Ops::mean(vector<FloatOp> ops) const { return mix(ops); }

FloatOp Ops::median(vector<FloatOp> ops) const {
  return [ops](float pos) {
    vector<float> values;
    for (auto op : ops) {
      values.push_back(op(pos));
    }
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
  };
}

FloatOp Ops::variance(vector<FloatOp> ops) const {
  return [ops, this](float pos) {
    float mn = mean(ops)(pos);
    float variance = 0.f;
    for (auto op : ops) {
      float diff = op(pos) - mn;
      variance += diff * diff;
    }
    variance /= ops.size();
    return variance;
  };
}

FloatOp Ops::stdDev(vector<FloatOp> ops) const {
  return [ops, this](float pos) { return std::sqrt(variance(ops)(pos)); };
}

FloatOp Ops::abs(FloatOp op) const {
  return [op](float pos) { return std::abs(op(pos)); };
}

FloatOp Ops::diff(FloatOp opA, FloatOp opB) const {
  return [opA, opB](float pos) { return opA(pos) - opB(pos); };
}

FloatOp Ops::greater(FloatOp opA, FloatOp opB) const {
  return [opA, opB](float pos) { return opA(pos) > opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::greater(FloatOp opA, float threshold) const {
  return
      [opA, threshold](float pos) { return opA(pos) > threshold ? 1.f : 0.f; };
}

FloatOp Ops::less(FloatOp opA, FloatOp opB) const {
  return [opA, opB](float pos) { return opA(pos) < opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::less(FloatOp opA, float threshold) const {
  return
      [opA, threshold](float pos) { return opA(pos) < threshold ? 1.f : 0.f; };
}

FloatOp Ops::equal(FloatOp opA, FloatOp opB) const {
  return [opA, opB](float pos) { return opA(pos) == opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::equal(FloatOp opA, float threshold) const {
  return
      [opA, threshold](float pos) { return opA(pos) == threshold ? 1.f : 0.f; };
}

FloatOp Ops::notEqual(FloatOp opA, FloatOp opB) const {
  return [opA, opB](float pos) { return opA(pos) != opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::notEqual(FloatOp opA, float threshold) const {
  return
      [opA, threshold](float pos) { return opA(pos) != threshold ? 1.f : 0.f; };
}

FloatOp Ops::and_(FloatOp opA, FloatOp opB, float threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return a > threshold && b > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::and_(FloatOp opA, FloatOp opB, FloatOp threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    float t = threshold(pos);
    return a > t || b > t ? 1.f : 0.f;
  };
}

FloatOp Ops::or_(FloatOp opA, FloatOp opB, float threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return a > threshold || b > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::or_(FloatOp opA, FloatOp opB, FloatOp threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    float t = threshold(pos);
    return a > t || b > t ? 1.f : 0.f;
  };
}

FloatOp Ops::not_(FloatOp op) const {
  return [op](float pos) { return op(pos) == 0.f ? 1.f : 0.f; };
}

FloatOp Ops::xor_(FloatOp opA, FloatOp opB, float threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return (a > threshold && b <= threshold) ||
                   (a <= threshold && b > threshold)
               ? 1.f
               : 0.f;
  };
}

FloatOp Ops::xor_(FloatOp opA, FloatOp opB, FloatOp threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    float t = threshold(pos);
    return (a > t && b <= t) || (a <= t && b > t) ? 1.f : 0.f;
  };
}

FloatOp Ops::nand(FloatOp opA, FloatOp opB, float threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return a > threshold && b > threshold ? 0.f : 1.f;
  };
}

FloatOp Ops::nand(FloatOp opA, FloatOp opB, FloatOp threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    float t = threshold(pos);
    return a > t || b > t ? 0.f : 1.f;
  };
}

FloatOp Ops::nor(FloatOp opA, FloatOp opB, float threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return a > threshold || b > threshold ? 0.f : 1.f;
  };
}

FloatOp Ops::nor(FloatOp opA, FloatOp opB, FloatOp threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    float t = threshold(pos);
    return a > t || b > t ? 0.f : 1.f;
  };
}

FloatOp Ops::xnor(FloatOp opA, FloatOp opB, float threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return (a > threshold && b > threshold) ||
                   (a <= threshold && b <= threshold)
               ? 1.f
               : 0.f;
  };
}

FloatOp Ops::xnor(FloatOp opA, FloatOp opB, FloatOp threshold) const {
  return [opA, opB, threshold](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    float t = threshold(pos);
    return (a > t && b > t) || (a <= t && b <= t) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(FloatOp op, float lo, float hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val >= lo && val <= hi ? 1.f : 0.f;
  };
}

FloatOp Ops::in(FloatOp op, FloatOp lo, FloatOp hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val >= lo(pos) && val <= hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(FloatOp op, float lo, FloatOp hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val >= lo && val <= hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(FloatOp op, FloatOp lo, float hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val >= lo(pos) && val <= hi ? 1.f : 0.f;
  };
}

FloatOp Ops::out(FloatOp op, float lo, float hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val < lo || val > hi ? 1.f : 0.f;
  };
}

FloatOp Ops::out(FloatOp op, FloatOp lo, FloatOp hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val < lo(pos) || val > hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::out(FloatOp op, float lo, FloatOp hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val < lo || val > hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::out(FloatOp op, FloatOp lo, float hi) const {
  return [op, lo, hi](float pos) {
    float val = op(pos);
    return val < lo(pos) || val > hi ? 1.f : 0.f;
  };
}

FloatOp Ops::chain(vector<FloatOp> ops) const {
  return [ops](float pos) {
    float val = pos;
    for (auto op : ops) {
      val = op(val);
    }
    return val;
  };
}

FloatOp Ops::choose(vector<FloatOp> ops) const {
  return [ops](float pos) {
    int index = static_cast<int>(ofRandom(ops.size()));
    return ops[index](pos);
  };
}

vector<float> Ops::normalize(vector<float> values) const {
  // Find min and max using std::min_element and std::max_element
  auto minMax = std::minmax_element(values.begin(), values.end());
  float min = *minMax.first;
  float max = *minMax.second;

  // Normalize the values
  std::vector<float> normValues;
  normValues.reserve(values.size());
  for (float v : values) {
    normValues.push_back((v - min) / (max - min));
  }

  return normValues;
}

FloatOp Ops::timeseries(vector<float> yValues) const {
  vector<float> normValues = normalize(yValues);
  return [normValues](float pos) {
    int index = static_cast<int>(pos * (normValues.size() - 1));
    float fraction = pos * (normValues.size() - 1) - index;
    return (normValues[index] * (1.f - fraction)) +
           (normValues[index + 1] * fraction);
  };
}

vector<float> Ops::floatArray(FloatOp op, int numSamples, FloatOp mapOp) const {
  float step = 1.f / numSamples;
  vector<float> table(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    float pos = i * step;
    table[i] = op(pos / numSamples);
  }
  if (!mapOp) return table;
  vector<float> mappedTable(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    mappedTable[i] = mapOp(table[i]);
  }
  return mappedTable;
}

vector<glm::vec2> Ops::glv2Array(FloatOp curve, float start, float end,
                                 int numPoints, float yScale) const {
  vector<glm::vec2> points(numPoints);
  float step = (end - start) / numPoints;
  end = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    float x = start + (i * step);
    float y = curve(x / end);
    points[i] = glm::vec2(x, ofGetHeight() - (y * yScale));
  }
  return points;
}

vector<glm::vec3> Ops::glv3Array(FloatOp curve, float start, float end,
                                 int numPoints, float yScale) const {
  vector<glm::vec3> points(numPoints);
  float step = (end - start) / numPoints;
  end = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    float x = start + (i * step);
    float y = curve(x / end);
    points[i] = glm::vec3(x, ofGetHeight() - (y * yScale), 0.f);
  }
  return points;
}

vector<ofVec2f> Ops::ofv2Array(FloatOp curve, float start, float end,
                               int numPoints, float yScale) const {
  vector<ofVec2f> points(numPoints);
  float step = (end - start) / numPoints;
  end = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    float x = start + (i * step);
    float y = curve(x / end);
    points[i] = ofVec2f(x, ofGetHeight() - (y * yScale));
  }
  return points;
}

vector<ofVec3f> Ops::ofv3Array(FloatOp curve, float start, float end,
                               int numPoints, float yScale) const {
  vector<ofVec3f> points(numPoints);
  float step = (end - start) / numPoints;
  end = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    float x = start + (i * step);
    float y = curve(x / end);
    points[i] = ofVec3f(x, ofGetHeight() - (y * yScale), 0.f);
  }
  return points;
}

float Ops::triDist(float lo, float hi, float mode) const {
  float F = (mode - lo) / (hi - lo);
  float rand = ofRandom(1.f);
  if (rand < F) {
    return lo + std::sqrt(rand * (hi - lo) * (mode - lo));
  } else {
    return hi - std::sqrt((1.f - rand) * (hi - lo) * (hi - mode));
  }
}

float Ops::pNoise(float x, float y, float z, float falloff, int octaves) const {
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float total = 0.0f;
  float maxAmplitude = 0.0f;  // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency, y * frequency, z * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

float Ops::pNoise(float x, float y, float falloff, int octaves) const {
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float total = 0.0f;
  float maxAmplitude = 0.0f;  // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency, y * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

float Ops::pNoise(float x, float falloff, int octaves) const {
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float total = 0.0f;
  float maxAmplitude = 0.0f;  // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

void Ops::plot(FloatOp op, float yScale, ofColor color, bool fill) const {
  ofPushStyle();
  ofSetColor(color);
  ofSetLineWidth(1);
  if (fill)
    ofFill();
  else
    ofNoFill();
  float width = ofGetWidth();
  vector<glm::vec2> points = glv2Array(op, 0.f, width, width, yScale);
  ofBeginShape();
  for (auto point : points) {
    ofVertex(point);
  }
  ofEndShape();
  ofPopStyle();
}

}  // namespace ofxCrvs
