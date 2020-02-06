//
// Created by emb on 2/3/20.
//

#include "Envelope.hpp"

int main() {
    Envelope env;
    env.setValue(0);
    env.setRiseShape(easing::function::in_exponential);
    env.setFallShape(easing::function::out_exponential);
    env.addStage(1.0, 1.0);
}