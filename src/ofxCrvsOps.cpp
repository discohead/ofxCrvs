
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
    if (s)
      sValue = s(pos);
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
    if (falloff)
      fof = falloff(pos);
    if (octaves)
      lod = octaves(pos);
    float xVal = x(pos);
    if (!y)
      return pNoise(xVal, fof, lod);
    float yVal = y(pos);
    if (!z)
      return pNoise(xVal, yVal, fof, lod);
    float zVal = z(pos);
    return pNoise(xVal, yVal, zVal, fof, lod);
  };
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
  float maxAmplitude = 0.0f; // Used for normalizing result to 0.0 - 1.0

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
  float maxAmplitude = 0.0f; // Used for normalizing result to 0.0 - 1.0

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
  float maxAmplitude = 0.0f; // Used for normalizing result to 0.0 - 1.0

  for (int i = 0; i < octaves; i++) {
    total += ofNoise(x * frequency) * amplitude;

    maxAmplitude += amplitude;
    amplitude *= falloff;
    frequency *= 2.0f;
  }

  return total / maxAmplitude;
}

} // namespace ofxCrvs
