//
// Created by emb on 2/3/20.
//

#ifndef DSP_KIT_ENVELOPE_HPP
#define DSP_KIT_ENVELOPE_HPP

#include <functional>
#include "easing.hpp"

class Envelope {
public:
    struct Stage {
        easing::function shape;
        float start;
        float end;
        float time;
    };

    enum class MoveState {
        idle,
        moving
    };

    enum class LoopMode {
        // treat stages as a fixed, looping sequence
                sequence,
        // consume each stage (FIFO), then stop
                queue
    };

    // fill an output buffer with values
public:
    MoveState moveState;

    float processSample();

    void addStage(float target, float time, easing::function shape = easing::function::none);
    void removeFirstStage();
    void removeLastStage();
    void clearAllStages();
};


#endif //DSP_KIT_ENVELOPE_HPP
