
#include "ofxCrvsOps.h"

namespace ofxCrvs {

float Ops::pos2Rad(float pos) {
  pos = ofClamp(pos, 0.f, 1.f);
  float degrees = pos * 360.f;
  return ofDegToRad(degrees);
  return pos;
}

FloatOp Ops::bipolarize(FloatOp unipolarOp) {
  return [unipolarOp](float pos) { return 2.f * unipolarOp(pos) - 1.f; };
}

FloatOp Ops::rectify(FloatOp bipolarOp) {
  return [bipolarOp](float pos) {
    float bipolarValue = bipolarOp(pos);
    return bipolarValue * 0.5f + 0.5f;
  };
}

FloatOp Ops::c(float value) {
  return [value](float /*pos*/) { return value; };
}

FloatOp Ops::phasor() {
  return [](float pos) { return pos; };
}

FloatOp Ops::saw() {
  return [](float pos) { return 1.f - pos; };
}

FloatOp Ops::tri(FloatOp s) {
  return [s](float pos) {
    float sValue = 0.5f;
    if (s) sValue = s(pos);
    return pos < sValue ? pos / sValue
                        : 1.f - ((pos - sValue) / (1.f - sValue));
  };
}

FloatOp Ops::tri() { return tri(nullptr); }

FloatOp Ops::tri(float s) { return tri(c(s)); }

FloatOp Ops::sine(FloatOp fb) {
  return [fb, this](float pos) {
    if (fb) {
      float fbScale = fb(pos);
      pos = pos + fbScale * (std::sin(pos2Rad(pos)) * 0.5f) + 0.5f;
    }
    return (std::sin(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::sine() { return sine(nullptr); }

FloatOp Ops::sine(float fb) { return sine(c(fb)); }

FloatOp Ops::asin() {
  return [](float pos) {
    pos = pos * 2.f - 1.f;
    return (std::asin(pos) + glm::half_pi<float>()) / glm::pi<float>();
  };
}

FloatOp Ops::cos(FloatOp fb) {
  return [fb, this](float pos) {
    if (fb) {
      float fbScale = fb(pos);
      pos = pos + fbScale * (std::cos(pos2Rad(pos)) * 0.5f) + 0.5f;
    }
    return (std::cos(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::cos() { return cos(nullptr); }

FloatOp Ops::cos(float fb) { return cos(c(fb)); }

FloatOp Ops::acos() {
  return [](float pos) {
    pos = pos * 2.f - 1.f;
    return std::acos(pos) / glm::pi<float>();
  };
}

FloatOp Ops::tan(FloatOp fb) {
  return [fb, this](float pos) {
    if (fb) {
      float fbScale = fb(pos);
      pos = pos + fbScale * (std::tan(pos2Rad(pos)) * 0.5f) + 0.5f;
    }
    return (std::tan(pos2Rad(fmod(pos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::tan() { return tan(nullptr); }

FloatOp Ops::tan(float fb) { return tan(c(fb)); }

FloatOp Ops::pulse(FloatOp w) {
  return [w](float pos) {
    float wValue = 0.5f;
    if (w) wValue = w(pos);
    return pos < wValue ? 0.f : 1.f;
  };
}

FloatOp Ops::pulse() { return pulse(nullptr); }

FloatOp Ops::pulse(float w) { return pulse(c(w)); }

FloatOp Ops::square() { return pulse(); }

FloatOp Ops::easeIn(FloatOp e) {
  return [e](float pos) {
    float eValue = 2.f;
    if (e) eValue = e(pos);
    return std::pow(pos, eValue);
  };
}

FloatOp Ops::easeIn() { return easeIn(nullptr); }

FloatOp Ops::easeIn(float e) { return easeIn(c(e)); }

FloatOp Ops::easeOut(FloatOp e) {
  return [e](float pos) {
    float eValue = 3.f;
    if (e) eValue = e(pos);
    return 1.f - std::pow((1.f - pos), eValue);
  };
}

FloatOp Ops::easeOut() { return easeOut(nullptr); }

FloatOp Ops::easeOut(float e) { return easeOut(c(e)); }

FloatOp Ops::easeInOut(FloatOp e) {
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

FloatOp Ops::easeInOut() { return easeInOut(nullptr); }

FloatOp Ops::easeInOut(float e) { return easeInOut(c(e)); }

FloatOp Ops::easeOutIn(FloatOp e) {
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

FloatOp Ops::easeOutIn() { return easeOutIn(nullptr); }

FloatOp Ops::easeOutIn(float e) { return easeOutIn(c(e)); }

FloatOp Ops::gaussian(FloatOp lo, FloatOp hi) {
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

FloatOp Ops::random(FloatOp lo, FloatOp hi, FloatOp mode) {
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

FloatOp Ops::wavetable() {
  return [this](float pos) {
    int t = static_cast<int>(ofMap(pos, 0.f, 1.f, 0.f, table.size()));
    float samp = table[t];
    return (samp + 1.f) * 0.5f;
  };
}

FloatOp Ops::perlin(FloatOp x, FloatOp y, FloatOp z, FloatOp falloff,
                    FloatOp octaves) {
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

FloatOp Ops::mult(FloatOp op, float scalar) {
  return [op, scalar](float pos) { return op(pos) * scalar; };
}

FloatOp Ops::bias(FloatOp op, float offset) {
  return [op, offset](float pos) { return op(pos) + offset; };
}

FloatOp Ops::bias(FloatOp op, FloatOp offset) {
  return [op, offset](float pos) { return op(pos) + offset(pos); };
}

FloatOp Ops::phase(FloatOp op, float phaseOffset) {
  return [op, phaseOffset](float pos) {
    pos = pos + phaseOffset;
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::phase(FloatOp op, FloatOp phaseOffset) {
  return [op, phaseOffset](float pos) {
    pos = pos + phaseOffset(pos);
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::rate(FloatOp op, float rateOffset) {
  return [op, rateOffset](float pos) {
    pos = pos * rateOffset;
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::rate(FloatOp op, FloatOp rateOffset) {
  return [op, rateOffset](float pos) {
    pos = pos * rateOffset(pos);
    if (pos > 1.f) pos = fmod(pos, 1.f);
    return op(pos);
  };
}

FloatOp Ops::ring(FloatOp opA, FloatOp opB) {
  return [opA, opB](float pos) {
    float a = opA(pos);
    float b = opB(pos);
    return a * b;
  };
}

FloatOp Ops::fold(FloatOp op, FloatOp threshold) {
  return [op, threshold](float pos) {
    float val = op(pos);
    float thresh = threshold(pos);
    while (val > thresh) {
      val = thresh - (val - thresh);
    }
    return val;
  };
}

FloatOp Ops::fold(FloatOp op, float threshold) {
  return [op, threshold](float pos) {
    float val = op(pos);
    while (val > threshold) {
      val = threshold - (val - threshold);
    }
    return val;
  };
}

FloatOp Ops::fold(FloatOp op) {
  return [op](float pos) {
    float val = op(pos);
    while (val > 1.f) {
      val = 1.f - (val - 1.f);
    }
    return val;
  };
}

FloatOp Ops::lpf(FloatOp inputOp, int windowSize) {
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

FloatOp Ops::chain(vector<FloatOp> ops) {
  return [ops](float pos) {
    float val = pos;
    for (auto op : ops) {
      val = op(val);
    }
    return val;
  };
}

FloatOp Ops::choose(vector<FloatOp> ops) {
  return [ops](float pos) {
    int index = static_cast<int>(ofRandom(ops.size()));
    return ops[index](pos);
  };
}

vector<float> Ops::normalize(vector<float> values) {
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

FloatOp Ops::timeseries(vector<float> yValues) {
  vector<float> normValues = normalize(yValues);
  return [normValues](float pos) {
    int index = static_cast<int>(pos * (normValues.size() - 1));
    float fraction = pos * (normValues.size() - 1) - index;
    return (normValues[index] * (1.f - fraction)) +
           (normValues[index + 1] * fraction);
  };
}

vector<float> Ops::floatArray(FloatOp op, int numSamples, FloatOp mapOp) {
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
                                 int numPoints, float yScale) {
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
                                 int numPoints, float yScale) {
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
                               int numPoints, float yScale) {
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
                               int numPoints, float yScale) {
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

float Ops::triDist(float lo, float hi, float mode) {
  float F = (mode - lo) / (hi - lo);
  float rand = ofRandom(1.f);
  if (rand < F) {
    return lo + std::sqrt(rand * (hi - lo) * (mode - lo));
  } else {
    return hi - std::sqrt((1.f - rand) * (hi - lo) * (hi - mode));
  }
}

float Ops::pNoise(float x, float y, float z, float falloff, int octaves) {
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

float Ops::pNoise(float x, float y, float falloff, int octaves) {
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

float Ops::pNoise(float x, float falloff, int octaves) {
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

void Ops::plot(FloatOp op, float yScale, ofColor color, bool fill) {
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
