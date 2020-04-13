#pragma once

//#include "Utilities.h"
#include "Lut.hpp"

namespace dspkit {
    class Taper {
    public:

        // low-resolution reverse audio taper for VU meters
        class Vu {
        private:
            static const float ampPosTable[];
            static const unsigned int ampPosTableSize;
        public:
            // get position on perceptual taper scale (IEC-60268)
            // assumption: amp >= 0.f
            static float getPos(float amp) {
                // FIXME: should we return a special value for clipping?
                if (amp > 1.f) { return 1.f; }
                return Lut<float>::lookupLinear(amp, ampPosTable, ampPosTableSize);
            }
        };

        // high-resolution audio taper for attenuation (0-1 amplitude.)
        // (gain can simply be added later if needed)
        class LevelControl {
        private:
            static const float posAmpTable[];
            static const unsigned int posAmpTableSize;
        public:
            static float getAmp(float pos) {
                if (pos > 1.f) { return 1.f; }
                if (pos < 0.f) { return 0.f; }
                return Lut<float>::lookupLinearNoClamp(pos, posAmpTable, posAmpTableSize);
            }
        };
    };
}