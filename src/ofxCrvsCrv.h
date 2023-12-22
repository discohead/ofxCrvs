
#pragma once

#include "ofxCrvsOps.h"
#include "ofxCrvsWindow.hpp"
#include "ofxCrvsEdg.hpp"

namespace ofxCrvs {
class Edg;
/**
 * The enum Component.
 */
enum class Component {
    /**
     * X component.
     */
    X,
    
    /**
     * Y component.
     */
    Y
};

enum class Bounding {
    NONE,
    CLIPPING,
    WRAPPING,
    FOLDING,
};


class Crv {
public:
    constexpr static glm::vec2 uCenter = glm::vec2(0.5f, 0.5f);
    
    FloatOp op;
    std::shared_ptr<Crv> amp;
    std::shared_ptr<Crv> rate;
    std::shared_ptr<Crv> phase;
    std::shared_ptr<Crv> bias;
    
    float ampOffset = 1.0f;
    float rateOffset = 1.0f;
    float phaseOffset = 0.0f;
    float biasOffset = 0.0f;
    
    int resolution;
    int quantization;
    glm::vec2 origin;
    glm::vec2 translation;
    glm::vec2 scale;
    float rotation;
    
    Bounding bounding;
    Window window;
    
    static std::shared_ptr<Crv> create() {
        return std::make_shared<Crv>();
    }
    
    static std::shared_ptr<Crv> create(FloatOp op) {
        return std::make_shared<Crv>(op);
    }
    
    static std::shared_ptr<Crv> create(FloatOp op,
                                       std::shared_ptr<Crv> amp,
                                       std::shared_ptr<Crv> rate,
                                       std::shared_ptr<Crv> phase,
                                       std::shared_ptr<Crv> bias)
    {
        return std::make_shared<Crv>(op, amp, rate, phase, bias);
    }
    
    static std::shared_ptr<Crv> create(FloatOp op, float ampOffset, float rateOffset, float phaseOffset, float biasOffset)
    {
        return std::make_shared<Crv>(op, ampOffset, rateOffset, phaseOffset, biasOffset);
    }
    
    static std::shared_ptr<Crv> create(Window window,
                                       FloatOp op,
                                       std::shared_ptr<Crv> amp,
                                       std::shared_ptr<Crv> rate,
                                       std::shared_ptr<Crv> phase,
                                       std::shared_ptr<Crv> bias,
                                       float ampOffset,
                                       float rateOffset,
                                       float phaseOffset,
                                       float biasOffset)
    {
        return std::make_shared<Crv>(window, op, amp, rate, phase, bias, ampOffset, rateOffset, phaseOffset, biasOffset);
    }
    
    Crv() : Crv(nullptr, nullptr, nullptr, nullptr, nullptr) {}
    
    Crv(Window window) : Crv(window, nullptr, nullptr, nullptr, nullptr, nullptr, 1.0f, 1.0f, 0.0f, 0.0f) {}
    
    Crv(FloatOp op) : Crv(op, nullptr, nullptr, nullptr, nullptr) {}
    
    Crv(FloatOp op,
        std::shared_ptr<Crv> amp,
        std::shared_ptr<Crv> rate,
        std::shared_ptr<Crv> phase,
        std::shared_ptr<Crv> bias)
    : Crv(Window(0, 0, ofGetWidth(), ofGetHeight()), op, amp, rate, phase, bias, 1.0f, 1.0f, 0.0f, 0.0f) {}
    
    Crv(FloatOp op, float ampOffset, float rateOffset, float phaseOffset, float biasOffset)
    : Crv(Window(0, 0, ofGetWidth(), ofGetHeight()), op, nullptr, nullptr, nullptr, nullptr, ampOffset, rateOffset, phaseOffset, biasOffset) {}
    
    Crv(Window window,
        FloatOp op,
        std::shared_ptr<Crv> amp,
        std::shared_ptr<Crv> rate,
        std::shared_ptr<Crv> phase,
        std::shared_ptr<Crv> bias,
        float ampOffset,
        float rateOffset,
        float phaseOffset,
        float biasOffset)
    : window(window), op(op), amp(amp), rate(rate), phase(phase), bias(bias), ampOffset(ampOffset), rateOffset(rateOffset), phaseOffset(phaseOffset), biasOffset(biasOffset)
    {
        resolution = window.width;
        quantization = 0;
        origin = glm::vec2(0.0f, 0.0f);
        translation = glm::vec2(0.0f, 0.0f);
        scale = glm::vec2(1.0f, 1.0f);
        rotation = 0.0f;
        if (!op) this->op = [](float x) { return x; };
    }
    
    float apply(float pos);
    float yAt(float pos);
    float xAt(float pos);
    float bipolarize(float unipolar);
    float wrap(float value, float min, float max);
    float fold(float value, float min, float max);
    float rectify(float bipolar);
    
    std::vector<float> floatArray(int numSamples, Component component);
    std::vector<float> floatArray(int numSamples);
    std::vector<glm::vec2> vectorArray(int numPoints, bool windowed, bool transformed, FloatOp samplingRateOp);
    std::vector<std::vector<float>> pointArray(int numPoints, bool windowed, bool transformed, FloatOp samplingRateOp);
    ofPolyline polyline(int numPoints, bool windowed, bool transformed, FloatOp samplingRateOp);
    
    glm::vec2 uVector(float pos, bool transformed);
    glm::vec2 wVector(float pos, bool transformed);
    
    glm::vec2 windowed(glm::vec2 v);
    glm::vec2 transform(glm::vec2 v);
    glm::vec2 bounded(glm::vec2 v);
    glm::vec2 clipped(glm::vec2 v, float min, float max);
    glm::vec2 clipped(glm::vec2 v);
    glm::vec2 wrapped(glm::vec2 v);
    glm::vec2 folded(glm::vec2 v);
    
    std::vector<Edg> getWebEdgs(int numPoints, bool windowed, bool transformed, int resolution);
    std::vector<Edg> getWebEdgs(int numPoints, bool windowed, bool transformed);
    
protected:
    ofBaseApp* app = ofGetAppPtr();
    float calculate(float pos);
    float ampBias(float value, float pos);
    float calcPos(float pos);
    virtual float componentAt(Component component, float pos);
    float quantize(float y);
};
} // namespace ofxCrvs
