#ifndef CRONE_FASTFADER_HPP
#define CRONE_FASTFADER_HPP

#include <cmath>
#include <limits>

namespace dspkit {
    class FastFader {
    private:
        static constexpr int tableSize = 1025;
        static constexpr float posMax = static_cast<float>(tableSize) - std::numeric_limits<float>::epsilon() ;
        static const float table[];

        enum {
            Rising, Falling, Stopped
        };
        int state;

        float time;
        float inc;

        float sr;
        float fTarget;
        int iTarget;
        float fCurPos;
        int iCurPos;

        void rise() {
            fCurPos += inc;
            if (fCurPos > fTarget) {
                fCurPos = fTarget;
                iCurPos = iTarget;
                state = Stopped;
                return;
            }
            iCurPos = (int) fCurPos;
        }

        void fall() {
            fCurPos += inc;
            if (fCurPos < fTarget) {
                fCurPos = fTarget;
                iCurPos = iTarget;
                state = Stopped;
                return;
            }
            iCurPos = (int) fCurPos;
        }

    public:
        FastFader() : state(Stopped),
                      fCurPos(0.f), iTarget(0), iCurPos(0),
                      fTarget(0.f), inc(1), time(0), sr(48000.f) {}

        void setSampleRate(float sr_) {
            sr = sr_;
        }

        void setTime(float t) {
            time = t;
        }

        void setTarget(float pos) {
            fTarget = std::fmax(0.f, std::fmin(posMax, pos * (float) tableSize));
            iTarget = (int) fTarget;
            inc = (fTarget - fCurPos) / (time * sr);
            state = iTarget == iCurPos ? Stopped :
                    (inc > 0.f ? Rising : Falling);
        }

        float next() {
            if (state == Rising) { rise(); }
            else if (state == Falling) { fall(); }
            return table[iCurPos];
        }
    };
}

#endif
