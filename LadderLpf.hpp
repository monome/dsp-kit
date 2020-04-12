// a 4th-order lowpass ladder filter
// after v. zavalashin and w. pirkle

#include <array>
#include <cmath>

#include <iostream>

#include "Lut.hpp"
#include "Constants.hpp"

namespace dspkit {
    template<typename T>
    class LadderLpf {
        T t;    // sampling interval
        T g;    // "small-g" single-stage tuning coefficient
        T k;    // scaling coefficient from Q
        T G;    // "big-G" coefficient for each stage

        std::array<T, 4> z1; // storage element per stage

        // table of
        static constexpr size_t gTabSize = 1024;
        // lookup table for primary coefficient (pitch-wise)
        std::array<float, gTabSize> gTab;

    public:
        void init(T sr) {
            t = 1 / sr;
        }

        void initPitchTable(double sr, double baseFreq, double numOct) {
            this->sr = static_cast<float>(sr);
            auto sz_1 = gTabSize -1;
            // base-2 logarithmic pitch increment per entry
            double inc = numOct / static_cast<double>(sz_1);
            double exp = 0.0;
            double f;
            for (int i = 0; i < sz_1; ++i) {
                f = baseFreq * pow(2.0, exp);
                // std::cout << f << std::endl;
                exp += inc;
                T wd = Constants::twopi * f;
                T wa = 2 / t * tan(wd * t / 2);
                gTab[i] = wa * t / 2;
            }
            // extra element for "extended" lookup
            gTab[sz_1] = gTab[sz_1 - 1];
        }

        void setCutoffPitch(float pitch) {
            this->g = Lut<float>::lookupLinear(pitch, gTab.data(), gTabSize);
            G = g / (1 + g);
        }

        void setCutoff(T fc) {
            T wd = Constants::twopi * fc;
            T wa = 2 / t * tan(wd * t / 2);
            g = wa * t / 2;
            G = g / (1 + g);
        }

        void setQ(T q) {
            // assuming Q in (0, ~20]
            k = 4 * (q - 0.707) / (25 - 0.707);
        }

        T stage(T x, T &z) {
            T v = G * (x - z);
            T y = v + z;
            z = y + v;
            return y;
        }

        T processSample(T x) {
            T g2 = g;
            T g3 = g2 * g;
            T g4 = g2 * g2;
            // sum all stage memories
            T S = g3 * z1[0] + g2 * z1[1] + g * z1[2] + z1[3];
            // scale and warp input as per zavalishin
            T u = (x - k * S) / (1 + k * g4);
            // cascade
            return stage(stage(stage(stage(u, z1[0]), z1[1]), z1[2]), z1[3]);
        }

    };

}