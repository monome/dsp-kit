#ifndef DSPKIT_SVF_H
#define DSPKIT_SVF_H

#include <memory>

namespace dspkit {

    class Svf {
    public:
        // c-tor / d-tor
        Svf();

        // initialize internal lookup table for linear pitch control
        // @param sr: sample rate
        // @param baseFreq: base frequency (pitch == 0)
        // @param numOct: how many octaves to compute above base frequency
        void initPitchTable(double sr, double baseFreq = 14.1, double numOct = 10.f, int size = 1024);

        // clear the internal fiter state
        void clear();

        // process one input sample, returning one output sample
        float processSample(float x);

        // set the corner frequency as linear pitch
        // mapping is dependent on parameters passed to `initPitchTable()`
        // @param pitch: linear pitch in [0, 1]
        void setFcPitch(float pitch);

        void setSr(float sr);
        void setFc(float fc);
        void setRq(float rq);
        void setLpMix(float mix);
        void setHpMix(float mix);
        void setBpMix(float mix);
        void setBrMix(float mix);

    private:
        void update(float x);

        // recalculate all coefficients for current fc, sr, rq
        void calcCoeffs();

        // recalculate cheaper conefficients for RQ only (no `tan`)
        void calcSecondaryCoeffs();

    private:
        // heap-allocated lookup table for primary coefficient (pitch-wise)
        std::unique_ptr<float[]> gtab;
        size_t gtabSize;

        // sample rate
        float sr;
        // corner frequency in hz
        float fc;
        // reciprocal of Q in [0,1]
        float rq;
        // intermediate coefficients
        float g;
        float g1;
        float g2;
        float g3;
        float g4;
        // state variables
        float v0;
        float v1;
        float v2;
        float v0z;
        float v1z;
        float v2z;
        float v3;
        // outputs
        float lp; // lowpass
        float hp; // highpass
        float bp; // bandpass
        float br; // bandreject
        // mix
        float lpMix;
        float hpMix;
        float bpMix;
        float brMix;
    };
}

#endif
