//
// Created by emb on 5/17/20.
//
// a pinking filter
//
// based on code by Paul Kellet from musicdsp.org
// (link is, alas, lost to time...)

#ifndef DSP_KIT_PINKER_HPP
#define DSP_KIT_PINKER_HPP

class Pinker {

    float b0;
    float b1;
    float b2;
    float b3;
    float b4;
    float b5;
    float b6;

    void init() {
        b0 = b1 = b2 = b3 = b4 = b5 = b6 = 0.f;
    }

    float update(float x) {
        b0 = 0.99886 * b0 + input[i] * 0.0555179;
        b1 = 0.99332 * b1 + input[i] * 0.0750759;
        b2 = 0.96900 * b2 + input[i] * 0.1538520;
        b3 = 0.86650 * b3 + input[i] * 0.3104856;
        b4 = 0.55000 * b4 + input[i] * 0.5329522;
        b5 = -0.7616 * b5 - input[i] * 0.0168980;
        y = b0 + b1 + b2 + b3 + b4 + b5 + b6 + x * 0.5362;
        y *= 0.11;
        b6 = x * 0.115926;
        return y;
    }
};

#endif //DSP_KIT_PINKER_HPP
