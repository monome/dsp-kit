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
        float sr;

        float time;
        float inc;

        float fCurPos;
        int iCurPos;
        float fTarget;
        int iTarget;

        int state;

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
        FastFader() :
                sr(48000.f),
                time(0), inc(1),
                fCurPos(0.f), iCurPos(0),
                fTarget(0.f), iTarget(0),
                state(Stopped) {}

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

        void setPos(float pos) {
            fCurPos = std::fmax(0.f, std::fmin(posMax, pos * (float) tableSize));
            iCurPos = (int)fCurPos;
            state = Stopped;
        }

        void update() {
            if (state == Rising) { rise(); }
            else if (state == Falling) { fall(); }
        }

        float getNextValue() {
            update();
            return table[iCurPos];
        }

        float getCurrentValue() {
            return table[iCurPos];
        }
    };
}

#endif
