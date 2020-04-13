//
// Created by emb on 2/3/20.
//

#include <cmath>
#include <iostream>

#include "Envelope.hpp"

using namespace dspkit;

/// FIXME: should be template with data type parameter
Envelope::Envelope() : sr(48000.f) {
    callback = nullptr;
    value = 0.f;
    start = 0.f;
    end = 0.f;
    inc = 0.f;
    scale = 0.f;
    moveState = MoveState::idle;
}

void Envelope::setSampleRate(float sr) {
    this->sr = sr;
}

float Envelope::processSample() {
    if (moveState == MoveState::moving) {
        updateValue();
        updatePhase();
    } else {
        if (stageQ.size() > 0) {
            nextStage();
        }
    }
    return value;
}

void Envelope::addStage(float target, float time, easing::function shape) {
    Stage stage{target, time, shape};
    stageQ.push(stage);
}


void Envelope::popStage() {
    Stage stage{};
    stageQ.pop(stage);
}

void Envelope::clearStages() {
    stageQ.clear();
}

void Envelope::nextStage() {
    Stage stage{};
    switch (sequenceMode) {
        case SequenceMode::queue:
            if (stageQ.pop(stage)) {
                go(stage.target, stage.time, stage.shape);
            }
            break;
        case SequenceMode::loop:
            // FIXME: use explicit indexing (i guess)
            if (stageQ.pop(stage)) {
                stageQ.push(stage);
                go(stage.target, stage.time, stage.shape);
            }
            break;
        case SequenceMode::oneshot:
            // TODO, needs indexing
            break;
    }
}

void Envelope::go(float target, float time, easing::function shape) {
    bool rising = target >= value;
    start = value;
    end = target;
    scale = end - start;
    float denom = fmaxf(time*sr, 1.f/scale);
    inc = 1.f / denom;
    phase = 0.f;

    std::cout << "going; start=" << value << "; end=" << target << "; scale=" << scale << "; inc = " << inc << std::endl;

    moveState = MoveState::moving;
    if (shape == easing::function::none) {
        shapeFunc = easing::get<float>(rising ? this->riseShape : this->fallShape);
    }
}

void Envelope::updateValue() {
    value = start + scale * shapeFunc(phase);
}

void Envelope::updatePhase() {
    phase += inc;
    if (phase > 1.f) {
        phase = 1.f;
        // NB: not this function's job to decide when to go to the next stage.
        // if we end in the idle state, next sample update should advance/loop/stop.
        moveState = MoveState::idle;
        if (callback != nullptr) {
            callback(value);
        }
    }
}

void Envelope::setSequenceMode(Envelope::SequenceMode mode) {
    sequenceMode = mode;
}

float Envelope::setRiseShape(easing::function shape) {
    riseShape = shape;
}

float Envelope::setFallShape(easing::function shape) {
    fallShape = shape;
}

void Envelope::setValue(float val) {
    value = val;
    end = val;
    moveState = MoveState::idle;
}

float Envelope::setCallback(std::function<void(float)> fn) {
    callback = fn;
}
