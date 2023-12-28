
#include "ofxCrvsOps.h"

namespace ofxCrvs {

float Ops::pos2Rad(const float pos) {
  return ofDegToRad(ofClamp(pos, 0.f, 1.f) * 360.f);
}

FloatOp Ops::bipolarize(const FloatOp &unipolarOp) const {
  return [unipolarOp](const float pos) { return 2.f * unipolarOp(pos) - 1.f; };
}

FloatOp Ops::rectify(const FloatOp &bipolarOp) const {
  return [bipolarOp](const float pos) { return bipolarOp(pos) * 0.5f + 0.5f; };
}

FloatOp Ops::c(const float value) const {
  return [value](const float /*pos*/) { return value; };
}

FloatOp Ops::phasor() const {
  return [](const float pos) { return pos; };
}

FloatOp Ops::saw() const {
  return [](const float pos) { return 1.f - pos; };
}

FloatOp Ops::tri(const FloatOp &s) const {
  return [s](const float pos) {
    float sValue = 0.5f;
    if (s)
      sValue = s(pos);
    return pos < sValue ? pos / sValue
                        : 1.f - ((pos - sValue) / (1.f - sValue));
  };
}

FloatOp Ops::tri() const { return tri(FloatOp()); }

FloatOp Ops::tri(const float s) const { return tri(c(s)); }

FloatOp Ops::sine(const FloatOp &fb) const {
  return [fb](const float pos) {
    float modPos = pos;
    if (fb) {
      const float fbScale = fb(modPos);
      modPos += fbScale * (std::sin(pos2Rad(modPos)) * 0.5f) + 0.5f;
    }
    return (std::sin(pos2Rad(fmod(modPos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::sine() const { return sine(FloatOp()); }

FloatOp Ops::sine(const float fb) const { return sine(c(fb)); }

FloatOp Ops::asin() const {
  return [](const float pos) {
    const float modPos = pos * 2.f - 1.f;
    return (std::asin(modPos) + glm::half_pi<float>()) / glm::pi<float>();
  };
}

FloatOp Ops::cos(const FloatOp &fb) const {
  return [fb](const float pos) {
    float modPos = pos;
    if (fb) {
      const float fbScale = fb(pos);
      modPos += fbScale * (std::cos(pos2Rad(modPos)) * 0.5f) + 0.5f;
    }
    return (std::cos(pos2Rad(fmod(modPos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::cos() const { return cos(FloatOp()); }

FloatOp Ops::cos(const float fb) const { return cos(c(fb)); }

FloatOp Ops::acos() const {
  return [](const float pos) {
    const float modPos = pos * 2.f - 1.f;
    return std::acos(modPos) / glm::pi<float>();
  };
}

FloatOp Ops::tan(const FloatOp &fb) const {
  return [fb](const float pos) {
    float modPos = pos;
    if (fb) {
      const float fbScale = fb(pos);
      modPos += fbScale * (std::tan(pos2Rad(modPos)) * 0.5f) + 0.5f;
    }
    return (std::tan(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::tan() const { return tan(FloatOp()); }

FloatOp Ops::tan(const float fb) const { return tan(c(fb)); }

FloatOp Ops::pulse(const FloatOp &w) const {
  return [w](const float pos) {
    float wValue = 0.5f;
    if (w)
      wValue = w(pos);
    return pos < wValue ? 0.f : 1.f;
  };
}

FloatOp Ops::pulse(const float w) const { return pulse(c(w)); }

FloatOp Ops::square() const { return pulse(); }

FloatOp Ops::easeIn(const FloatOp &e) const {
  return [e](const float pos) {
    float eValue = 2.f;
    if (e)
      eValue = e(pos);
    return std::pow(pos, eValue);
  };
}

FloatOp Ops::easeIn() const { return easeIn(FloatOp()); }

FloatOp Ops::easeIn(const float e) const { return easeIn(c(e)); }

FloatOp Ops::easeOut(const FloatOp &e) const {
  return [e](const float pos) {
    float eValue = 3.f;
    if (e)
      eValue = e(pos);
    return 1.f - std::pow((1.f - pos), eValue);
  };
}

FloatOp Ops::easeOut() const { return easeOut(FloatOp()); }

FloatOp Ops::easeOut(const float e) const { return easeOut(c(e)); }

FloatOp Ops::easeInOut(const FloatOp &e) const {
  return [e](const float pos) {
    const float value = pos * 2.f;
    float eValue = 3.f;
    if (e)
      eValue = e(pos);
    if (value > 1.f) {
      return 0.5f * std::pow(value, eValue);
    } else {
      return 0.5f * (2.f - std::pow((2.f - value), eValue));
    }
  };
}

FloatOp Ops::easeInOut() const { return easeInOut(FloatOp()); }

FloatOp Ops::easeInOut(const float e) const { return easeInOut(c(e)); }

FloatOp Ops::easeOutIn(const FloatOp &e) const {
  return [e](const float pos) {
    float value = pos * 2.f;
    float eValue = 3.f;
    if (e)
      eValue = e(pos);
    if (value < 1.f) {
      return (1.f - std::pow((1.f - value), eValue) * 0.5f) - 0.5f;
    } else {
      value = value - 1.f;
      return (std::pow(value, eValue) * 0.5f) + 0.5f;
    }
  };
}

FloatOp Ops::easeOutIn() const { return easeOutIn(FloatOp()); }

FloatOp Ops::easeOutIn(const float e) const { return easeOutIn(c(e)); }

// Envelope Ops
FloatOp Ops::env(const float attackLength, const float attackLevel,
                 const float decayLength, const float sustainLength,
                 const float sustainLevel, const float releaseLength) const {
  return [attackLength, attackLevel, decayLength, sustainLength, sustainLevel,
          releaseLength](const float pos) {
    if (pos <= attackLength) {
      return attackLevel * (pos / attackLength);
    } else if (pos <= attackLength + decayLength) {
      return attackLevel + ((pos - attackLength) / decayLength *
                            (sustainLevel - attackLevel));
    } else if (pos <= attackLength + decayLength + sustainLength) {
      return sustainLevel;
    } else if (pos <=
               attackLength + decayLength + sustainLength + releaseLength) {
      return sustainLevel -
             ((pos - (attackLength + decayLength + sustainLength)) /
              releaseLength * sustainLevel);
    } else {
      return 0.f;
    }
  };
}

FloatOp Ops::breakpoints(const vector<vector<float>> &points) const {
  return [points](const float pos) {
    for (int i = 0; i < points.size(); ++i) {
      if (pos < points[i][0]) {
        if (i == 0)
          return points[i][1];
        const float prevPos = points[i - 1][0];
        const float prevVal = points[i - 1][1];
        const float nextPos = points[i][0];
        const float nextVal = points[i][1];
        const float fraction = (pos - prevPos) / (nextPos - prevPos);
        return prevVal + (fraction * (nextVal - prevVal));
      }
    }
    return 0.f;
  };
}

FloatOp Ops::gaussian(const FloatOp &lo, const FloatOp &hi) const {
  return [lo, hi](const float pos) {
    float g = ofRandomGaussian(0.f, 1.f);
    if (g < -1.f)
      g = fmod(g, -1.f);
    else if (g > 1.f)
      g = fmod(g, 1.f);
    g = (g + 1.f) * 0.5f;
    const float loVal = lo ? lo(pos) : 0.f;
    const float hiVal = hi ? hi(pos) : 1.f;
    return loVal + (g * (hiVal - loVal));
  };
}

FloatOp Ops::random(const FloatOp &lo, const FloatOp &hi,
                    const FloatOp &mode) const {
  return [lo, hi, mode, this](const float pos) {
    const float loVal = lo ? lo(pos) : 0.f;
    const float hiVal = hi ? hi(pos) : 1.f;
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
  return [this](const float pos) {
    const int t = static_cast<int>(ofMap(pos, 0.f, 1.f, 0.f, table.size()));
    const float samp = table[t];
    return (samp + 1.f) * 0.5f;
  };
}

FloatOp Ops::perlin(const FloatOp &x, const FloatOp &y, const FloatOp &z,
                    const FloatOp &falloff, const FloatOp &octaves) const {
  return [x, y, z, falloff, octaves, this](const float pos) {
    int lod = 1;
    float fof = 1.f;
    if (falloff)
      fof = falloff(pos);
    if (octaves)
      lod = octaves(pos);
    const float xVal = x(pos);
    if (!y)
      return pNoise(xVal, fof, lod);
    const float yVal = y(pos);
    if (!z)
      return pNoise(xVal, yVal, fof, lod);
    const float zVal = z(pos);
    return pNoise(xVal, yVal, zVal, fof, lod);
  };
}

FloatOp Ops::mult(const FloatOp &op, const float scalar) const {
  return [op, scalar](const float pos) { return op(pos) * scalar; };
}

FloatOp Ops::bias(const FloatOp &op, const float offset) const {
  return [op, offset](const float pos) { return op(pos) + offset; };
}

FloatOp Ops::bias(const FloatOp &op, const FloatOp &offset) const {
  return [op, offset](const float pos) { return op(pos) + offset(pos); };
}

FloatOp Ops::phase(const FloatOp &op, const float phaseOffset) const {
  return [op, phaseOffset](const float pos) {
    float modPos = pos + phaseOffset;
    if (modPos > 1.f)
      modPos = fmod(pos, 1.f);
    return op(modPos);
  };
}

FloatOp Ops::phase(const FloatOp &op, const FloatOp &phaseOffset) const {
  return [op, phaseOffset](const float pos) {
    float modPos = pos + phaseOffset(pos);
    if (modPos > 1.f)
      modPos = fmod(pos, 1.f);
    return op(modPos);
  };
}

FloatOp Ops::rate(const FloatOp &op, const float rateOffset) const {
  return [op, rateOffset](const float pos) {
    float modPos = pos * rateOffset;
    if (modPos > 1.f)
      modPos = fmod(pos, 1.f);
    return op(modPos);
  };
}

FloatOp Ops::rate(const FloatOp &op, const FloatOp &rateOffset) const {
  return [op, rateOffset](const float pos) {
    float modPos = pos * rateOffset(pos);
    if (modPos > 1.f)
      modPos = fmod(pos, 1.f);
    return op(modPos);
  };
}

FloatOp Ops::ring(const FloatOp &opA, const FloatOp &opB) const {
  return [opA, opB](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a * b;
  };
}

FloatOp Ops::fold(const FloatOp &op, const FloatOp &threshold) const {
  return [op, threshold](const float pos) {
    float val = op(pos);
    const float thresh = threshold(pos);
    while (val > thresh) {
      val = thresh - (val - thresh);
    }
    return val;
  };
}

FloatOp Ops::fold(const FloatOp &op, const float threshold) const {
  return [op, threshold](const float pos) {
    float val = op(pos);
    while (val > threshold) {
      val = threshold - (val - threshold);
    }
    return val;
  };
}

FloatOp Ops::fold(const FloatOp &op) const {
  return [op](const float pos) {
    float val = op(pos);
    while (val > 1.f) {
      val = 1.f - (val - 1.f);
    }
    return val;
  };
}

FloatOp Ops::lpf(const FloatOp &inputOp, const int windowSize) const {
  return [inputOp, windowSize](const float pos) {
    float sum = 0.f;
    for (int i = 0; i < windowSize; ++i) {
      float offsetPos = pos - (static_cast<float>(i) / windowSize);
      offsetPos = std::max(0.f, std::min(1.f, offsetPos));
      sum += inputOp(offsetPos);
    }
    return sum / windowSize;
  };
}

FloatOp Ops::mix(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    float sum = 0.f;
    for (const auto &op : ops) {
      sum += op(pos);
    }
    return sum / ops.size();
  };
}

FloatOp Ops::mix(const vector<FloatOp> &ops,
                 const vector<float> &levels) const {
  return [ops, levels](const float pos) {
    float sum = 0.f;
    for (int i = 0; i < ops.size(); ++i) {
      sum += ops[i](pos) * levels[i];
    }
    return sum / ops.size();
  };
}

FloatOp Ops::mix(const vector<FloatOp> &ops,
                 const vector<FloatOp> &levels) const {
  return [ops, levels](const float pos) {
    float sum = 0.f;
    for (int i = 0; i < ops.size(); ++i) {
      sum += ops[i](pos) * levels[i](pos);
    }
    return sum / ops.size();
  };
}

FloatOp Ops::sum(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    float sum = 0.f;
    for (const auto &op : ops) {
      sum += op(pos);
    }
    return sum;
  };
}

FloatOp Ops::product(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    float product = 1.f;
    for (const auto &op : ops) {
      product *= op(pos);
    }
    return product;
  };
}

FloatOp Ops::min(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    float min = std::numeric_limits<float>::max();
    for (const auto &op : ops) {
      if (const float val = op(pos); val < min)
        min = val;
    }
    return min;
  };
}

FloatOp Ops::max(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    float max = std::numeric_limits<float>::min();
    for (const auto &op : ops) {
      if (const float val = op(pos); val > max)
        max = val;
    }
    return max;
  };
}

FloatOp Ops::mean(const vector<FloatOp> &ops) const { return mix(ops); }

FloatOp Ops::median(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    vector<float> values;
    values.reserve(ops.size());

    for (const auto &op : ops) {
      values.push_back(op(pos));
    }
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
  };
}

FloatOp Ops::variance(const vector<FloatOp> &ops) const {
  return [ops, this](const float pos) {
    const float mn = mean(ops)(pos);
    float variance = 0.f;
    for (const auto &op : ops) {
      const float diff = op(pos) - mn;
      variance += diff * diff;
    }
    variance /= ops.size();
    return variance;
  };
}

FloatOp Ops::stdDev(const vector<FloatOp> &ops) const {
  return [ops, this](const float pos) { return std::sqrt(variance(ops)(pos)); };
}

FloatOp Ops::abs(const FloatOp &op) const {
  return [op](const float pos) { return std::abs(op(pos)); };
}

FloatOp Ops::diff(const FloatOp &opA, const FloatOp &opB) const {
  return [opA, opB](const float pos) { return opA(pos) - opB(pos); };
}

FloatOp Ops::greater(const FloatOp &opA, const FloatOp &opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) > opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::greater(const FloatOp &opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::less(const FloatOp &opA, const FloatOp &opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) < opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::less(const FloatOp &opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) < threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::equal(const FloatOp &opA, const FloatOp &opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) == opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::equal(const FloatOp &opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) == threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::notEqual(const FloatOp &opA, const FloatOp &opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) != opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::notEqual(const FloatOp &opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) != threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::and_(const FloatOp &opA, const FloatOp &opB,
                  const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold && b > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::and_(const FloatOp &opA, const FloatOp &opB,
                  const FloatOp &threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 1.f : 0.f;
  };
}

FloatOp Ops::or_(const FloatOp &opA, const FloatOp &opB,
                 const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold || b > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::or_(const FloatOp &opA, const FloatOp &opB,
                 const FloatOp &threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 1.f : 0.f;
  };
}

FloatOp Ops::not_(const FloatOp &op) const {
  return [op](const float pos) { return op(pos) == 0.f ? 1.f : 0.f; };
}

FloatOp Ops::xor_(const FloatOp &opA, const FloatOp &opB,
                  const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return (a > threshold && b <= threshold) ||
                   (a <= threshold && b > threshold)
               ? 1.f
               : 0.f;
  };
}

FloatOp Ops::xor_(const FloatOp &opA, const FloatOp &opB,
                  const FloatOp &threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return (a > t && b <= t) || (a <= t && b > t) ? 1.f : 0.f;
  };
}

FloatOp Ops::nand(const FloatOp &opA, const FloatOp &opB,
                  const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold && b > threshold ? 0.f : 1.f;
  };
}

FloatOp Ops::nand(const FloatOp &opA, const FloatOp &opB,
                  const FloatOp &threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 0.f : 1.f;
  };
}

FloatOp Ops::nor(const FloatOp &opA, const FloatOp &opB,
                 const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold || b > threshold ? 0.f : 1.f;
  };
}

FloatOp Ops::nor(const FloatOp &opA, const FloatOp &opB,
                 const FloatOp &threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 0.f : 1.f;
  };
}

FloatOp Ops::xnor(const FloatOp &opA, const FloatOp &opB,
                  const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return (a > threshold && b > threshold) ||
                   (a <= threshold && b <= threshold)
               ? 1.f
               : 0.f;
  };
}

FloatOp Ops::xnor(const FloatOp &opA, const FloatOp &opB,
                  const FloatOp &threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return (a > t && b > t) || (a <= t && b <= t) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp &op, const float lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo && val <= hi ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp &op, const FloatOp &lo, const FloatOp &hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo(pos) && val <= hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp &op, const float lo, const FloatOp &hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo && val <= hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp &op, const FloatOp &lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo(pos) && val <= hi ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp &op, const float lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo || val > hi ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp &op, const FloatOp &lo,
                 const FloatOp &hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo(pos) || val > hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp &op, const float lo, const FloatOp &hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo || val > hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp &op, const FloatOp &lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo(pos) || val > hi ? 1.f : 0.f;
  };
}

FloatOp Ops::chain(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    float val = pos;
    for (const auto &op : ops) {
      val = op(val);
    }
    return val;
  };
}

FloatOp Ops::choose(const vector<FloatOp> &ops) const {
  return [ops](const float pos) {
    const int index = static_cast<int>(ofRandom(ops.size()));
    return ops[index](pos);
  };
}

vector<float> Ops::normalize(const vector<float> &values) const {
  // Find min and max using structured bindings
  const auto [minIt, maxIt] = std::minmax_element(values.begin(), values.end());
  const float min = *minIt;
  const float max = *maxIt;

  // Normalize the values
  std::vector<float> normValues;
  normValues.reserve(values.size());
  for (const float v : values) {
    normValues.push_back((v - min) / (max - min));
  }

  return normValues;
}

FloatOp Ops::timeseries(const vector<float> &yValues) const {
  vector<float> normValues = normalize(yValues);
  return [normValues](const float pos) {
    const int index = static_cast<int>(pos * (normValues.size() - 1));
    const float fraction = pos * (normValues.size() - 1) - index;
    return (normValues[index] * (1.f - fraction)) +
           (normValues[index + 1] * fraction);
  };
}

vector<float> Ops::floatArray(const FloatOp &op, const int numSamples,
                              const FloatOp &mapOp) const {
  const float step = 1.f / numSamples;
  vector<float> localTable(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    const float pos = i * step;
    localTable[i] = op(pos / numSamples);
  }
  if (!mapOp)
    return localTable;
  vector<float> mappedTable(numSamples);
  for (int i = 0; i < numSamples; ++i) {
    mappedTable[i] = mapOp(localTable[i]);
  }
  return mappedTable;
}

vector<glm::vec2> Ops::glv2Array(const FloatOp &curve, const float start,
                                 const float end, const int numPoints,
                                 const float yScale) const {
  vector<glm::vec2> points(numPoints);
  const float step = (end - start) / numPoints;
  const float modEnd = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    const float x = start + (i * step);
    const float y = curve(x / modEnd);
    points[i] = glm::vec2(x, ofGetHeight() - (y * yScale));
  }
  return points;
}

vector<glm::vec3> Ops::glv3Array(const FloatOp &curve, const float start,
                                 const float end, const int numPoints,
                                 const float yScale) const {
  vector<glm::vec3> points(numPoints);
  const float step = (end - start) / numPoints;
  const float modEnd = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    const float x = start + (i * step);
    const float y = curve(x / modEnd);
    points[i] = glm::vec3(x, ofGetHeight() - (y * yScale), 0.f);
  }
  return points;
}

vector<ofVec2f> Ops::ofv2Array(const FloatOp &curve, const float start,
                               const float end, const int numPoints,
                               const float yScale) const {
  vector<ofVec2f> points(numPoints);
  const float step = (end - start) / numPoints;
  const float modEnd = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    const float x = start + (i * step);
    const float y = curve(x / modEnd);
    points[i] = ofVec2f(x, ofGetHeight() - (y * yScale));
  }
  return points;
}

vector<ofVec3f> Ops::ofv3Array(const FloatOp &curve, const float start,
                               const float end, const int numPoints,
                               const float yScale) const {
  vector<ofVec3f> points(numPoints);
  const float step = (end - start) / numPoints;
  const float modEnd = end - (step - 1);
  for (int i = 0; i < numPoints; ++i) {
    const float x = start + (i * step);
    const float y = curve(x / modEnd);
    points[i] = ofVec3f(x, ofGetHeight() - (y * yScale), 0.f);
  }
  return points;
}

float Ops::triDist(const float lo, const float hi, const float mode) const {
  const float F = (mode - lo) / (hi - lo);
  if (const float rand = ofRandom(1.f); rand < F) {
    return lo + std::sqrt(rand * (hi - lo) * (mode - lo));
  } else {
    return hi - std::sqrt((1.f - rand) * (hi - lo) * (hi - mode));
  }
}

float Ops::pNoise(const float x, const float y, const float z,
                  const float falloff, const int octaves) const {
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float total = 0.0f;
  float maxAmplitude = 0.0f; // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency, y * frequency, z * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

float Ops::pNoise(const float x, const float y, const float falloff,
                  int octaves) const {
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float total = 0.0f;
  float maxAmplitude = 0.0f; // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency, y * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

float Ops::pNoise(const float x, const float falloff, const int octaves) const {
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float total = 0.0f;
  float maxAmplitude = 0.0f; // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

void Ops::plot(const FloatOp &op, const float yScale, const ofColor color,
               const bool fill) const {
  ofPushStyle();
  ofSetColor(color);
  ofSetLineWidth(1);
  if (fill)
    ofFill();
  else
    ofNoFill();
  const float width = ofGetWidth();
  const vector<glm::vec2> points = glv2Array(op, 0.f, width, width, yScale);
  ofBeginShape();
  for (auto point : points) {
    ofVertex(point);
  }
  ofEndShape();
  ofPopStyle();
}

} // namespace ofxCrvs
