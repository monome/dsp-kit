#ifndef DSPKIT_SVF_H
#define DSPKIT_SVF_H

#include <array>
#include <memory>

namespace dspkit {

    class Svf {
    public:
        // c-tor / d-tor
        Svf();

        // clear the internal filter state
        void clear();

        // process one input sample, returning one output sample
        float processSample(float x);

        // set the corner frequency as linear pitch
        // uses pre-initialized table, so range of pitch can vary
        // @param pitch: linear pitch in [0, 1]
        void setCutoffPitchNoCalc(float pitch);

        // set table to use for pitch lookup
        // this allows multiple instances to share table memory
        void setGTable(const float* gainTable, int tableSize);

        // fill a table with gain coefficient values from linear pitch
        // takes minimum and maximum arguments as midi note numbers
        static void fillGTable(float* gainTable, int size, float sampleRate,
                               float midiMin=0, float midiMax=127.f);

        void setSampleRate(float sr);
        void setCutoff(float fc);
        void setInverseQ(float rq);
        void setCutoffNoCalc(float fc);
        void setInverseQNoCalc(float rq);
        void setLpMix(float mix);
        void setHpMix(float mix);
        void setBpMix(float mix);
        void setBrMix(float mix);

        // recalculate all coefficients for current fc, sr, rq
        void calcCoeffs();

        // calculate and return main coefficient given FC, SR, RQ
        static float getG(float sr, float fc);
        // set main coefficient directly
        void setG(float g);

        // recalculate cheaper conefficients for RQ only (no `tan`)
        void calcSecondaryCoeffs();

    private:
        // calculate new outputs from input
        void update(float x);

    private:
        // sample rate
        float sr{};
        float sr_2{};
        // corner frequency in hz
        float fc{};
        // reciprocal of Q in [0,1]
        float rq{};
        // intermediate coefficients
        float g{};
        float g1{};
        float g2{};
        float g3{};
        float g4{};
        // state variables
        float v0{};
        float v1{};
        float v2{};
        float v0z{};
        float v1z{};
        float v2z{};
        float v3{};
        // outputs
        float lp{}; // lowpass
        float hp{}; // highpass
        float bp{}; // bandpass
        float br{}; // bandreject
        // mix
        float lpMix{};
        float hpMix{};
        float bpMix{};
        float brMix{};
        // table mapping gain coefficients to pitch
        const float *gTable;
        int gTableSize;
    };
}

#endif
