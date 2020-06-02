// a 4th-order lowpass ladder filter
// after v. zavalashin and w. pirkle
#ifndef DSPKIT_LADDER_LPF_HPP
#define DSPKIT_LADDER_LPF_HPP

#include <array>
#include <cmath>

#include <iostream>

#include "Lut.hpp"
#include "Constants.hpp"

namespace dspkit {
    template<typename T>
    class LadderLpf {
        T sr;   // sampling rate
        T t;    // sampling interval (stored for speed)
        T g;    // "small-g" single-stage tuning coefficient
        T k;    // scaling coefficient from Q
        T G;    // "big-G" coefficient for each stage

        std::array<T, 4> z1; // storage element per stage

        const float* gTab = nullptr;
        int gTabSize;

    public:
        void init(T sr_) {
            sr = sr_;
            t = 1 / sr;
        }

        // calculate "little-g" coefficient (most expensive)
        static T getG(T sr, T fc) {
            const T t = 1/sr;
            const T wd = Constants::twopi * fc;
            const T wa = 2 * sr * tan(wd * t / 2);
            return wa * t / 2;
        }

        static void fillGTable(float* gTab, int gTabSize, double sr, double baseFreq, double numOct) {
            auto sz_1 = gTabSize -1;
            // base-2 logarithmic pitch increment per entry
            double inc = numOct / static_cast<double>(sz_1);
            double exp = 0.0;
            double f;
            for (int i = 0; i < sz_1; ++i) {
                f = baseFreq * pow(2.0, exp);
                exp += inc;
                gTab[i] = getG(sr, f);
            }
            // extra element for "extended" lookup
            gTab[sz_1] = gTab[sz_1 - 1];
        }

        void setGTable(const float* tab, int size) {
            gTab = tab;
            gTabSize = size;
        }

        // assumption: gTab is not null
        void setCutoffPitch(float pitch) {
            this->g = Lut<float>::lookupLinear(pitch, gTab, gTabSize);
            G = g / (1 + g);
        }

        void setCutoff(T fc) {
            T wd = Constants::twopi * fc;
            T wa = 2 * sr * tan(wd * t / 2);
            g = wa * t / 2;
            G = g / (1 + g);
        }

        void setQ(T q) {
            // assuming Q in (0, ~20]
            k = 4 * (q - 0.707) / (25 - 0.707);
        }

        inline T stage(T x, T &z) {
            const T v = G * (x - z);
            const T y = v + z;
            z = y + v;
            return y;
        }

        T processSample(T x) {
            const T g2 = g;
            const T g3 = g2 * g;
            const T g4 = g2 * g2;
            // sum all stage memories
            T S = g3 * z1[0] + g2 * z1[1] + g * z1[2] + z1[3];
            // scale and warp input as per zavalishin
            T u = (x - k * S) / (1 + k * g4);
            // cascade
            return stage(stage(stage(stage(u, z1[0]), z1[1]), z1[2]), z1[3]);
        }

    };

}

#endif // DSPKIT_LADDER_LPF_HPP