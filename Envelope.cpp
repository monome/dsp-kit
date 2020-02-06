//
// Created by emb on 2/3/20.
//

#include <cassert>
#include "Envelope.hpp"

void Envelope::setSampleRate(float sr) {
    this->sr = sr;
}

float Envelope::processSample() {
    if (moveState == MoveState::moving) {
        updateValue();
        updatePhase();
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
            // FIXME: use index
            if (stageQ.pop(stage)) {
                stageQ.push(stage);
                go(stage.target, stage.time, stage.shape);
            }
            break;
        case SequenceMode::oneshot:
            // TODO
            break;
    }
}

void Envelope::go(float target, float time, easing::function shape) {
    bool rising = target >= value;
    start = value;
    end = target;
    scale = target - value;
    inc = 1.f / (time * sr);
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
        moveState = MoveState::idle;
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
