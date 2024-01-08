
#include "ofxCrvsOps.h"

namespace ofxCrvs {

float Ops::pos2Rad(const float pos) {
  return ofDegToRad(ofClamp(pos, 0.f, 1.f) * 360.f);
}

FloatOp Ops::bipolarize(const FloatOp unipolarOp) const {
  return [unipolarOp](const float pos) { return 2.f * unipolarOp(pos) - 1.f; };
}

FloatOp Ops::rectify(const FloatOp bipolarOp) const {
  return [bipolarOp](const float pos) { return bipolarOp(pos) * 0.5f + 0.5f; };
}

FloatOp Ops::c(const float value) const {
  return [value](const float /*pos*/) { return value; };
}

FloatOp Ops::timePhasor(const double cycleDurationSeconds) const {
  if (cycleDurationSeconds <= 0.0) {
    throw std::invalid_argument("cycleDurationSeconds must be greater than 0");
  }
  double cycleDurationMicros = cycleDurationSeconds * 1000000.0;
  return [cycleDurationMicros](const float) {
    return fmod(static_cast<double>(ofGetElapsedTimeMicros()),
                cycleDurationMicros) /
           cycleDurationMicros;
  };
}

FloatOp Ops::tempoPhasor(const double barsPerCycle, const double bpm) const {
  if (bpm <= 0.0) {
    throw std::invalid_argument("bpm must be greater than 0");
  }
  if (barsPerCycle <= 0.0) {
    throw std::invalid_argument("barsPerCycle must be greater than 0");
  }

  // Calculate the duration of one beat in seconds
  const double beatDurationSecs = 60.0 / bpm;

  // Assuming a 4/4 time signature, calculate the duration of one bar
  const double barDurationSecs = beatDurationSecs * 4;

  // Calculate the duration of the entire cycle (in seconds)
  const double cycleDurationSecs = barDurationSecs * barsPerCycle;

  // Convert cycle duration from seconds to microseconds
  const double cycleDurationMicros = cycleDurationSecs * 1000000.0;

  return [cycleDurationMicros](const float) {
    return fmod(static_cast<double>(ofGetElapsedTimeMicros()),
                cycleDurationMicros) /
           cycleDurationMicros;
  };
}

FloatOp Ops::phasor() const {
  return [](const float pos) { return pos; };
}

FloatOp Ops::saw() const {
  return [](const float pos) { return 1.f - pos; };
}

FloatOp Ops::tri(const FloatOp s) const {
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

FloatOp Ops::sine(const FloatOp fb) const {
  return [fb](const float pos) {
    float modPos = pos;
    if (fb) {
      const float fbScale = fb(modPos);
      modPos += fbScale * (std::sin(pos2Rad(modPos)) * 0.5f) + 0.5f;
    }
    return (std::sin(pos2Rad(fmod(modPos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::sineFb(const FloatOp fb) const {
  return [fb, lastFeedback = 0.0f](const float pos) mutable -> float {
    float modPos = pos;

    // Calculate the current feedback scale
    float currentFeedback = fb ? fb(pos) : 0.0f;

    // Apply the feedback to modPos
    modPos += lastFeedback;

    // Compute the sine wave with modulated position
    float output = (std::sin(pos2Rad(fmod(modPos, 1.f))) * 0.5f) + 0.5f;

    // Update the lastFeedback for the next call
    // The feedback effect is based on the current output scaled by
    // currentFeedback
    lastFeedback = currentFeedback * output;

    return output;
  };
}

FloatOp Ops::sineFb(const float fb) const {
  return [fb, lastFeedback = 0.0f](const float pos) mutable -> float {
    float modPos = pos;

    // Apply the feedback to modPos
    modPos += lastFeedback;

    // Compute the sine wave with modulated position
    float output = (std::sin(pos2Rad(fmod(modPos, 1.f))) * 0.5f) + 0.5f;

    // Update the lastFeedback for the next call
    // The feedback effect is based on the current output scaled by fb
    lastFeedback = fb * output;

    return output;
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

FloatOp Ops::cos(const FloatOp fb) const {
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

FloatOp Ops::tan(const FloatOp fb) const {
  return [fb](const float pos) {
    float modPos = pos;
    if (fb) {
      const float fbScale = fb(pos);
      modPos += fbScale * (std::tan(pos2Rad(modPos)) * 0.5f) + 0.5f;
    }
    return (std::tan(pos2Rad(fmod(modPos, 1.f))) * 0.5f) + 0.5f;
  };
}

FloatOp Ops::tan() const { return tan(FloatOp()); }

FloatOp Ops::tan(const float fb) const { return tan(c(fb)); }

FloatOp Ops::pulse(const FloatOp w) const {
  return [w](const float pos) {
    float wValue = 0.5f;
    if (w)
      wValue = w(pos);
    return pos < wValue ? 0.f : 1.f;
  };
}

FloatOp Ops::pulse(const float w) const { return pulse(c(w)); }

FloatOp Ops::square() const { return pulse(); }

FloatOp Ops::easeIn(const FloatOp e) const {
  return [e](const float pos) {
    float eValue = 2.f;
    if (e)
      eValue = e(pos);
    return std::pow(pos, eValue);
  };
}

FloatOp Ops::easeIn() const { return easeIn(FloatOp()); }

FloatOp Ops::easeIn(const float e) const { return easeIn(c(e)); }

FloatOp Ops::easeOut(const FloatOp e) const {
  return [e](const float pos) {
    float eValue = 3.f;
    if (e)
      eValue = e(pos);
    return 1.f - std::pow((1.f - pos), eValue);
  };
}

FloatOp Ops::easeOut() const { return easeOut(FloatOp()); }

FloatOp Ops::easeOut(const float e) const { return easeOut(c(e)); }

FloatOp Ops::easeInOut(const FloatOp e) const {
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

FloatOp Ops::easeOutIn(const FloatOp e) const {
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

FloatOp Ops::breakpoints(const vector<vector<float>> points) const {
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

FloatOp Ops::gaussian(const FloatOp lo, const FloatOp hi) const {
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

FloatOp Ops::random(const FloatOp lo, const FloatOp hi,
                    const FloatOp mode) const {
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

FloatOp Ops::lookup(const std::vector<float> table) const {
  return [table](const float pos) {
    const int t = static_cast<int>(ofMap(pos, 0.f, 1.f, 0.f, table.size()));
    return table[t];
  };
}

FloatOp Ops::lookup(const std::vector<FloatOp> table) const {
  return [table](const float pos) {
    const int t = static_cast<int>(ofMap(pos, 0.f, 1.f, 0.f, table.size()));
    return table[t](pos);
  };
}

FloatOp Ops::wt(const std::vector<float> wTable) const {
  return [wTable](const float pos) {
    // Map pos to the range of the wavetable indices
    float exactPos = ofMap(pos, 0.f, 1.f, 0.f, wTable.size());

    // Determine the indices of the surrounding samples
    int index1 = static_cast<int>(exactPos) % wTable.size();
    int index2 = (index1 + 1) % wTable.size();

    // Calculate the fractional part of the position
    float fraction = exactPos - static_cast<float>(index1);
    // Linearly interpolate between the two samples using ofLerp
    return ofLerp(wTable[index1], wTable[index2], fraction);
  };
}

FloatOp Ops::wt(const std::vector<FloatOp> wTable) const {
  return [wTable](const float pos) {
    // Map pos to the range of the wavetable indices
    float exactPos = ofMap(pos, 0.f, 1.f, 0.f, wTable.size());

    // Determine the indices of the surrounding samples
    int index1 = static_cast<int>(exactPos) % wTable.size();
    int index2 = (index1 + 1) % wTable.size();

    // Calculate the fractional part of the position
    float fraction = exactPos - static_cast<float>(index1);
    // Linearly interpolate between the two samples using ofLerp
    return ofLerp(wTable[index1](pos), wTable[index2](pos), fraction);
  };
}

FloatOp Ops::wt(const std::vector<float> wTable, const FloatOp xOp) const {
  return [wTable, xOp](float pos) {
    // Map xOp to the range of the wavetable
    float xPos = ofMap(xOp(pos), 0.f, 1.f, 0.f, wTable.size() - 1);

    // Compute the lower index for the x axis
    std::size_t xIndex = static_cast<std::size_t>(xPos);

    // Compute the fractional part for the x axis
    float xFrac = xPos - xIndex;

    // Ensure index is within bounds
    std::size_t xIndexNext = std::min(xIndex + 1, wTable.size() - 1);

    // Linear interpolation
    return ofLerp(wTable[xIndex], wTable[xIndexNext], xFrac);
  };
}

FloatOp Ops::wt(const std::vector<FloatOp> wTable, const FloatOp xOp) const {
  return [wTable, xOp](float pos) {
    // Map xOp to the range of the wavetable
    float xPos = ofMap(xOp(pos), 0.f, 1.f, 0.f, wTable.size() - 1);

    // Compute the lower index for the x axis
    std::size_t xIndex = static_cast<std::size_t>(xPos);

    // Compute the fractional part for the x axis
    float xFrac = xPos - xIndex;

    // Ensure index is within bounds
    std::size_t xIndexNext = std::min(xIndex + 1, wTable.size() - 1);

    // Linear interpolation
    return ofLerp(wTable[xIndex](pos), wTable[xIndexNext](pos), xFrac);
  };
}

FloatOp Ops::wt2d(const std::vector<std::vector<float>> wTable,
                  const FloatOp xOp, const FloatOp yOp) const {
  return [wTable, xOp, yOp](float pos) {
    // Map xOp and yOp to their respective ranges
    float xPos = ofMap(xOp(pos), 0.f, 1.f, 0.f, wTable.size() - 1);
    float yPos = ofMap(yOp(pos), 0.f, 1.f, 0.f, wTable[0].size() - 1);

    // Compute the lower indices for each axis
    std::size_t xIndex = static_cast<std::size_t>(xPos);
    std::size_t yIndex = static_cast<std::size_t>(yPos);

    // Compute the fractional part for each axis
    float xFrac = xPos - xIndex;
    float yFrac = yPos - yIndex;

    // Ensure indices are within bounds
    std::size_t xIndexNext = std::min(xIndex + 1, wTable.size() - 1);
    std::size_t yIndexNext = std::min(yIndex + 1, wTable[0].size() - 1);

    // Bilinear interpolation
    float v00 = wTable[xIndex][yIndex];
    float v10 = wTable[xIndexNext][yIndex];
    float v01 = wTable[xIndex][yIndexNext];
    float v11 = wTable[xIndexNext][yIndexNext];

    float c0 = ofLerp(v00, v10, xFrac);
    float c1 = ofLerp(v01, v11, xFrac);

    return ofLerp(c0, c1, yFrac);
  };
}

FloatOp Ops::wt2d(const std::vector<std::vector<FloatOp>> wTable,
                  const FloatOp xOp, const FloatOp yOp) const {
  return [wTable, xOp, yOp](float pos) {
    // Map xOp and yOp to their respective ranges
    float xPos = ofMap(xOp(pos), 0.f, 1.f, 0.f, wTable.size() - 1);
    float yPos = ofMap(yOp(pos), 0.f, 1.f, 0.f, wTable[0].size() - 1);

    // Compute the lower indices for each axis
    std::size_t xIndex = static_cast<std::size_t>(xPos);
    std::size_t yIndex = static_cast<std::size_t>(yPos);

    // Compute the fractional part for each axis
    float xFrac = xPos - xIndex;
    float yFrac = yPos - yIndex;

    // Ensure indices are within bounds
    std::size_t xIndexNext = std::min(xIndex + 1, wTable.size() - 1);
    std::size_t yIndexNext = std::min(yIndex + 1, wTable[0].size() - 1);

    // Bilinear interpolation
    float v00 = wTable[xIndex][yIndex](pos);
    float v10 = wTable[xIndexNext][yIndex](pos);
    float v01 = wTable[xIndex][yIndexNext](pos);
    float v11 = wTable[xIndexNext][yIndexNext](pos);

    float c0 = ofLerp(v00, v10, xFrac);
    float c1 = ofLerp(v01, v11, xFrac);

    return ofLerp(c0, c1, yFrac);
  };
}

FloatOp Ops::wt3d(const std::vector<std::vector<std::vector<float>>> wTable,
                  const FloatOp xOp, const FloatOp yOp,
                  const FloatOp zOp) const {
  return [wTable, xOp, yOp, zOp](float pos) {
    // Get exact positions for each axis
    float xPos = ofMap(xOp(pos), 0.f, 1.f, 0.f, wTable.size() - 1);
    float yPos = ofMap(yOp(pos), 0.f, 1.f, 0.f, wTable[0].size() - 1);
    float zPos = ofMap(zOp(pos), 0.f, 1.f, 0.f, wTable[0][0].size() - 1);

    // Compute the lower indices for each axis
    std::size_t xIndex = static_cast<std::size_t>(xPos);
    std::size_t yIndex = static_cast<std::size_t>(yPos);
    std::size_t zIndex = static_cast<std::size_t>(zPos);

    // Compute the fractional part for each axis
    float xFrac = xPos - xIndex;
    float yFrac = yPos - yIndex;
    float zFrac = zPos - zIndex;

    // Ensure indices are within bounds
    std::size_t xIndexNext = std::min(xIndex + 1, wTable.size() - 1);
    std::size_t yIndexNext = std::min(yIndex + 1, wTable[0].size() - 1);
    std::size_t zIndexNext = std::min(zIndex + 1, wTable[0][0].size() - 1);

    // Trilinear interpolation
    float v000 = wTable[xIndex][yIndex][zIndex];
    float v100 = wTable[xIndexNext][yIndex][zIndex];
    float v010 = wTable[xIndex][yIndexNext][zIndex];
    float v001 = wTable[xIndex][yIndex][zIndexNext];
    float v101 = wTable[xIndexNext][yIndex][zIndexNext];
    float v011 = wTable[xIndex][yIndexNext][zIndexNext];
    float v110 = wTable[xIndexNext][yIndexNext][zIndex];
    float v111 = wTable[xIndexNext][yIndexNext][zIndexNext];

    float c00 = ofLerp(v000, v100, xFrac);
    float c01 = ofLerp(v001, v101, xFrac);
    float c10 = ofLerp(v010, v110, xFrac);
    float c11 = ofLerp(v011, v111, xFrac);

    float c0 = ofLerp(c00, c10, yFrac);
    float c1 = ofLerp(c01, c11, yFrac);

    return ofLerp(c0, c1, zFrac);
  };
}

FloatOp Ops::wt3d(const std::vector<std::vector<std::vector<FloatOp>>> wOpTable,
                  const FloatOp xOp, const FloatOp yOp,
                  const FloatOp zOp) const {
  return [wOpTable, xOp, yOp, zOp](float pos) {
    // Get exact positions for each axis
    float xPos = ofMap(xOp(pos), 0.f, 1.f, 0.f, wOpTable.size() - 1);
    float yPos = ofMap(yOp(pos), 0.f, 1.f, 0.f, wOpTable[0].size() - 1);
    float zPos = ofMap(zOp(pos), 0.f, 1.f, 0.f, wOpTable[0][0].size() - 1);

    // Compute the lower indices for each axis
    std::size_t xIndex = static_cast<std::size_t>(xPos);
    std::size_t yIndex = static_cast<std::size_t>(yPos);
    std::size_t zIndex = static_cast<std::size_t>(zPos);

    // Compute the fractional part for each axis
    float xFrac = xPos - xIndex;
    float yFrac = yPos - yIndex;
    float zFrac = zPos - zIndex;

    // Ensure indices are within bounds
    std::size_t xIndexNext = std::min(xIndex + 1, wOpTable.size() - 1);
    std::size_t yIndexNext = std::min(yIndex + 1, wOpTable[0].size() - 1);
    std::size_t zIndexNext = std::min(zIndex + 1, wOpTable[0][0].size() - 1);

    // Evaluate the FloatOp at each corner of the cube
    float v000 = wOpTable[xIndex][yIndex][zIndex](pos);
    float v100 = wOpTable[xIndexNext][yIndex][zIndex](pos);
    float v010 = wOpTable[xIndex][yIndexNext][zIndex](pos);
    float v001 = wOpTable[xIndex][yIndex][zIndexNext](pos);
    float v101 = wOpTable[xIndexNext][yIndex][zIndexNext](pos);
    float v011 = wOpTable[xIndex][yIndexNext][zIndexNext](pos);
    float v110 = wOpTable[xIndexNext][yIndexNext][zIndex](pos);
    float v111 = wOpTable[xIndexNext][yIndexNext][zIndexNext](pos);

    // Perform trilinear interpolation
    float c00 = ofLerp(v000, v100, xFrac);
    float c01 = ofLerp(v001, v101, xFrac);
    float c10 = ofLerp(v010, v110, xFrac);
    float c11 = ofLerp(v011, v111, xFrac);

    float c0 = ofLerp(c00, c10, yFrac);
    float c1 = ofLerp(c01, c11, yFrac);

    return ofLerp(c0, c1, zFrac);
  };
}

FloatOp Ops::perlin(const FloatOp x, const FloatOp y, const FloatOp z,
                    const FloatOp falloff, const FloatOp octaves) const {
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

FloatOp Ops::fuzz(const float fuzzScale) const {
  return [fuzzScale](const float pos) -> float {
    // Generate Perlin noise and scale it by fuzzScale
    float noise = ofNoise(pos) * fuzzScale;

    // Add the scaled noise to the input position
    return pos + noise;
  };
}

FloatOp Ops::mult(const FloatOp op, const float scalar) const {
  return [op, scalar](const float pos) { return op(pos) * scalar; };
}

FloatOp Ops::bias(const FloatOp op, const float offset) const {
  return [op, offset](const float pos) { return op(pos) + offset; };
}

FloatOp Ops::bias(const FloatOp op, const FloatOp offset) const {
  return [op, offset](const float pos) { return op(pos) + offset(pos); };
}

FloatOp Ops::phase(const FloatOp op, const float phaseOffset) const {
  return [op, phaseOffset](const float pos) {
    float modPos = pos + phaseOffset;
    if (modPos > 1.f) {
      modPos = fmod(modPos, 1.f);
    }
    return op(modPos);
  };
}

FloatOp Ops::phase(const FloatOp op, const FloatOp phaseOffset) const {
  return [op, phaseOffset](const float pos) {
    float modPos = pos + phaseOffset(pos);
    if (modPos > 1.f)
      modPos = fmod(modPos, 1.f);
    return op(modPos);
  };
}

FloatOp Ops::rate(const FloatOp op, float rateOffset) const {
  return [op, rateOffset, lastPos = 0.f,
          accumulatedPos = 0.f](const float pos) mutable {
    float deltaPos = pos - lastPos;
    lastPos = pos;
    if (deltaPos < 0.f)
      deltaPos += 1.f;
    float modDelta = deltaPos * rateOffset;
    accumulatedPos += modDelta;
    if (accumulatedPos > 1.f)
      accumulatedPos = fmod(accumulatedPos, 1.f);
    return op(accumulatedPos);
  };
}

FloatOp Ops::rate(const FloatOp op, const FloatOp rateOffset) const {
  return [op, rateOffset, lastPos = 0.f,
          accumulatedPos = 0.f](const float pos) mutable {
    float deltaPos = pos - lastPos;
    lastPos = pos;
    if (deltaPos < 0.f)
      deltaPos += 1.f;
    float modDelta = deltaPos * rateOffset(pos);
    accumulatedPos += modDelta;
    if (accumulatedPos > 1.f)
      accumulatedPos = fmod(accumulatedPos, 1.f);
    return op(accumulatedPos);
  };
}

FloatOp Ops::ring(const FloatOp opA, const FloatOp opB) const {
  return [opA, opB](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a * b;
  };
}

FloatOp Ops::wrap(const FloatOp op, float min, float max) const {
  return [op, min, max](const float pos) {
    const float val = op(pos);
    if (val < min)
      return max - (min - val);
    else if (val > max)
      return min + (val - max);
    return val;
  };
}

FloatOp Ops::wrap(const FloatOp op, const FloatOp minOp,
                  const FloatOp maxOp) const {
  return [op, minOp, maxOp](const float pos) {
    const float val = op(pos);
    const float minVal = minOp(pos);
    const float maxVal = maxOp(pos);
    if (val < minVal)
      return maxVal - (minVal - val);
    else if (val > maxVal)
      return minVal + (val - maxVal);
    return val;
  };
}

FloatOp Ops::fold(const FloatOp op, const FloatOp threshold) const {
  return [op, threshold](const float pos) {
    float val = op(pos);
    const float thresh = threshold(pos);
    while (val > thresh) {
      val = thresh - (val - thresh);
    }
    return val;
  };
}

FloatOp Ops::fold(const FloatOp op, const float threshold) const {
  return [op, threshold](const float pos) {
    float val = op(pos);
    while (val > threshold) {
      val = threshold - (val - threshold);
    }
    return val;
  };
}

FloatOp Ops::fold(const FloatOp op) const {
  return [op](const float pos) {
    float val = op(pos);
    while (val > 1.f) {
      val = 1.f - (val - 1.f);
    }
    return val;
  };
}

FloatOp Ops::lpf(const FloatOp inputOp, const int windowSize) const {
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

FloatOp Ops::lpFb(float smoothing, float resonance) const {
  return [smoothing, resonance,
          lastOutput = 0.0f](const float input) mutable -> float {
    // Calculate the feedback amount
    float feedback = resonance * lastOutput;

    // Apply the low-pass filter formula with resonance
    lastOutput =
        smoothing * (input + feedback) + (1.0f - smoothing) * lastOutput;

    // Ensure the output is bounded, if necessary
    lastOutput = std::clamp(lastOutput, -1.0f, 1.0f);

    return lastOutput;
  };
}

FloatOp Ops::ampFb(float feedbackStrength, float damping,
                   const FloatOp inputOp) const {
  return [inputOp, feedbackStrength, damping,
          lastOutput = 0.0f](const float pos) mutable -> float {
    // Get the current input value
    float input = inputOp ? inputOp(pos) : pos;

    // Apply feedback to the input
    float modulatedInput = input + lastOutput * feedbackStrength;

    // Compute the output with some damping to stabilize the feedback loop
    float output = modulatedInput * (1.0f - damping);

    // Update the last output for the next iteration
    lastOutput = output;

    return output;
  };
}

FloatOp Ops::morph(const FloatOp opA, const FloatOp opB,
                   const FloatOp morphParam) const {
  return [opA, opB, morphParam](const float pos) -> float {
    float blend = morphParam(pos);
    blend = std::clamp(blend, 0.0f, 1.0f); // Ensure blend stays within [0, 1]
    return (1.0f - blend) * opA(pos) + blend * opB(pos);
  };
}

FloatOp Ops::morph(const vector<FloatOp> ops, const FloatOp morphParam) const {
  return [ops, morphParam](const float pos) -> float {
    float blend = morphParam(pos);
    blend = std::clamp(blend, 0.0f, 1.0f); // Ensure blend stays within [0, 1]

    // Map pos to the range of the op indices
    float exactPos = ofMap(pos, 0.f, 1.f, 0.f, ops.size());

    // Determine the indices of the surrounding ops
    int index1 = static_cast<int>(exactPos) % ops.size();
    int index2 = (index1 + 1) % ops.size();

    // Calculate the fractional part of the position
    float fraction = exactPos - static_cast<float>(index1);
    // Linearly interpolate between the two ops using ofLerp
    return ofLerp(ops[index1](pos), ops[index2](pos), fraction);
  };
}

FloatOp Ops::mix(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    float sum = 0.f;
    for (const auto &op : ops) {
      sum += op(pos);
    }
    return sum / ops.size();
  };
}

FloatOp Ops::mix(const vector<FloatOp> ops, const vector<float> levels) const {
  return [ops, levels](const float pos) {
    float sum = 0.f;
    for (int i = 0; i < ops.size(); ++i) {
      sum += ops[i](pos) * levels[i];
    }
    return sum / ops.size();
  };
}

FloatOp Ops::mix(const vector<FloatOp> ops,
                 const vector<FloatOp> levels) const {
  return [ops, levels](const float pos) {
    float sum = 0.f;
    for (int i = 0; i < ops.size(); ++i) {
      sum += ops[i](pos) * levels[i](pos);
    }
    return sum / ops.size();
  };
}

FloatOp Ops::sum(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    float sum = 0.f;
    for (const auto &op : ops) {
      sum += op(pos);
    }
    return sum;
  };
}

FloatOp Ops::product(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    float product = 1.f;
    for (const auto &op : ops) {
      product *= op(pos);
    }
    return product;
  };
}

FloatOp Ops::min(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    float min = std::numeric_limits<float>::max();
    for (const auto &op : ops) {
      if (const float val = op(pos); val < min)
        min = val;
    }
    return min;
  };
}

FloatOp Ops::max(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    float max = std::numeric_limits<float>::min();
    for (const auto &op : ops) {
      if (const float val = op(pos); val > max)
        max = val;
    }
    return max;
  };
}

FloatOp Ops::mean(const vector<FloatOp> ops) const { return mix(ops); }

FloatOp Ops::median(const vector<FloatOp> ops) const {
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

FloatOp Ops::variance(const vector<FloatOp> ops) const {
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

FloatOp Ops::stdDev(const vector<FloatOp> ops) const {
  return [ops, this](const float pos) { return std::sqrt(variance(ops)(pos)); };
}

FloatOp Ops::smooth() const {
  return [](const float pos) -> float {
    // Clamp x to the range [0, 1]
    float clampedX = std::clamp(pos, 0.0f, 1.0f);
    return clampedX * clampedX * (3 - 2 * clampedX);
  };
}

FloatOp Ops::smoother() const {
  return [](const float pos) -> float {
    // Clamp x to the range [0, 1]
    float clampedX = std::clamp(pos, 0.0f, 1.0f);
    return clampedX * clampedX * clampedX *
           (clampedX * (clampedX * 6 - 15) + 10);
  };
}

FloatOp Ops::ema(float smoothingFactor) const {
  return
      [smoothingFactor, lastOutput = 0.0f](const float pos) mutable -> float {
        lastOutput = smoothingFactor * pos + (1 - smoothingFactor) * lastOutput;
        return lastOutput;
      };
}

FloatOp Ops::ema(const FloatOp smoothingFactor) const {
  return
      [smoothingFactor, lastOutput = 0.0f](const float pos) mutable -> float {
        const float smoothing = smoothingFactor(pos);
        lastOutput = smoothing * pos + (1 - smoothing) * lastOutput;
        return lastOutput;
      };
}

FloatOp Ops::abs(const FloatOp op) const {
  return [op](const float pos) { return std::abs(op(pos)); };
}

FloatOp Ops::diff(const FloatOp opA, const FloatOp opB) const {
  return [opA, opB](const float pos) { return opA(pos) - opB(pos); };
}

FloatOp Ops::crossed(const FloatOp opA, const FloatOp opB) const {
  return [opA, opB,
          lastComparison = std::optional<bool>()](const float pos) mutable {
    const float a = opA(pos);
    const float b = opB(pos);
    bool aGreaterThanB = a > b;

    if (!lastComparison.has_value()) {
      // Initialize the state during the first call
      lastComparison = aGreaterThanB;
      return 0.f;
    }

    if (aGreaterThanB != lastComparison) {
      // A crossing has occurred
      lastComparison = aGreaterThanB;
      return 1.f;
    }

    return 0.f; // No crossing
  };
}

FloatOp Ops::trendFlip(const FloatOp inputOp) const {
  return [inputOp, lastValue = std::optional<float>(),
          lastDirection =
              std::optional<bool>()](const float pos) mutable -> float {
    // Get the current value
    float currentValue = inputOp(pos);

    // Check if lastValue has been initialized
    if (!lastValue.has_value()) {
      lastValue = currentValue;
      return 0.0f; // No direction change can be detected on the first call
    }

    // Determine the current direction (true for increasing, false for
    // decreasing)
    bool currentDirection = currentValue > lastValue.value();

    // Check if lastDirection has been initialized
    if (!lastDirection.has_value()) {
      lastDirection = currentDirection;
      lastValue = currentValue;
      return 0.0f; // No direction change on the first valid comparison
    }

    // Check for a change in direction
    if (currentDirection != lastDirection.value()) {
      lastDirection = currentDirection;
      lastValue = currentValue;
      return 1.0f; // Direction change detected
    }

    // Update lastValue for the next call
    lastValue = currentValue;
    return 0.0f; // No change in direction
  };
}

FloatOp Ops::greater(const FloatOp opA, const FloatOp opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) > opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::greater(const FloatOp opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::less(const FloatOp opA, const FloatOp opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) < opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::less(const FloatOp opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) < threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::equal(const FloatOp opA, const FloatOp opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) == opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::equal(const FloatOp opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) == threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::notEqual(const FloatOp opA, const FloatOp opB) const {
  return
      [opA, opB](const float pos) { return opA(pos) != opB(pos) ? 1.f : 0.f; };
}

FloatOp Ops::notEqual(const FloatOp opA, const float threshold) const {
  return [opA, threshold](const float pos) {
    return opA(pos) != threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::and_(const FloatOp opA, const FloatOp opB,
                  const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold && b > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::and_(const FloatOp opA, const FloatOp opB,
                  const FloatOp threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t && b > t ? 1.f : 0.f;
  };
}

FloatOp Ops::or_(const FloatOp opA, const FloatOp opB,
                 const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold || b > threshold ? 1.f : 0.f;
  };
}

FloatOp Ops::or_(const FloatOp opA, const FloatOp opB,
                 const FloatOp threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 1.f : 0.f;
  };
}

FloatOp Ops::not_(const FloatOp op) const {
  return [op](const float pos) { return op(pos) == 0.f ? 1.f : 0.f; };
}

FloatOp Ops::xor_(const FloatOp opA, const FloatOp opB,
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

FloatOp Ops::xor_(const FloatOp opA, const FloatOp opB,
                  const FloatOp threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return (a > t && b <= t) || (a <= t && b > t) ? 1.f : 0.f;
  };
}

FloatOp Ops::nand(const FloatOp opA, const FloatOp opB,
                  const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold && b > threshold ? 0.f : 1.f;
  };
}

FloatOp Ops::nand(const FloatOp opA, const FloatOp opB,
                  const FloatOp threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 0.f : 1.f;
  };
}

FloatOp Ops::nor(const FloatOp opA, const FloatOp opB,
                 const float threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    return a > threshold || b > threshold ? 0.f : 1.f;
  };
}

FloatOp Ops::nor(const FloatOp opA, const FloatOp opB,
                 const FloatOp threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return a > t || b > t ? 0.f : 1.f;
  };
}

FloatOp Ops::xnor(const FloatOp opA, const FloatOp opB,
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

FloatOp Ops::xnor(const FloatOp opA, const FloatOp opB,
                  const FloatOp threshold) const {
  return [opA, opB, threshold](const float pos) {
    const float a = opA(pos);
    const float b = opB(pos);
    const float t = threshold(pos);
    return (a > t && b > t) || (a <= t && b <= t) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp op, const float lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo && val <= hi ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp op, const FloatOp lo, const FloatOp hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo(pos) && val <= hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp op, const float lo, const FloatOp hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo && val <= hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::in(const FloatOp op, const FloatOp lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val >= lo(pos) && val <= hi ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp op, const float lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo || val > hi ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp op, const FloatOp lo, const FloatOp hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo(pos) || val > hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp op, const float lo, const FloatOp hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo || val > hi(pos) ? 1.f : 0.f;
  };
}

FloatOp Ops::out(const FloatOp op, const FloatOp lo, const float hi) const {
  return [op, lo, hi](const float pos) {
    const float val = op(pos);
    return val < lo(pos) || val > hi ? 1.f : 0.f;
  };
}

FloatOp Ops::chain(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    float val = pos;
    for (const auto &op : ops) {
      val = op(val);
    }
    return val;
  };
}

FloatOp Ops::choose(const vector<FloatOp> ops) const {
  return [ops](const float pos) {
    const int index = static_cast<int>(ofRandom(ops.size()));
    return ops[index](pos);
  };
}

vector<float> Ops::normalize(const vector<float> values) const {
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

FloatOp Ops::timeseries(const vector<float> yValues) const {
  vector<float> normValues = normalize(yValues);
  return [normValues](const float pos) {
    const int index = static_cast<int>(pos * (normValues.size() - 1));
    const float fraction = pos * (normValues.size() - 1) - index;
    return (normValues[index] * (1.f - fraction)) +
           (normValues[index + 1] * fraction);
  };
}

vector<float> Ops::floatArray(const FloatOp op, const int numSamples,
                              const FloatOp mapOp) const {
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

vector<glm::vec2> Ops::glv2Array(const FloatOp curve, const float start,
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

vector<glm::vec3> Ops::glv3Array(const FloatOp curve, const float start,
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

vector<ofVec2f> Ops::ofv2Array(const FloatOp curve, const float start,
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

vector<ofVec3f> Ops::ofv3Array(const FloatOp curve, const float start,
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

void Ops::plot(const FloatOp op, const float yScale, const ofColor color,
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
