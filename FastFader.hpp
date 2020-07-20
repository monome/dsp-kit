#ifndef CRONE_FASTFADER_HPP
#define CRONE_FASTFADER_HPP

#include <cmath>
#include <limits>

namespace dspkit {
    class FastFader {
    private:
        static constexpr int tableSize = 1025;
        // FIXME: was attempting to create a float value such that:
        // - casting to int results in (tableSize - 1)
        // - maximum value is as close as possible to (tableSize).
        // 2nd condition proving difficult
        static constexpr float posMax = static_cast<float>(tableSize) - 1 + 0.5f;
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
            iCurPos = static_cast<int>(fCurPos);
        }

        void fall() {
            fCurPos += inc;
            if (fCurPos < fTarget) {
                fCurPos = fTarget;
                iCurPos = iTarget;
                state = Stopped;
                return;
            }
            iCurPos = static_cast<int>(fCurPos);
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
            if (time <= std::numeric_limits<float>::epsilon()) {
                setPos(fTarget);
            } else {
                inc = time > 0 ? (fTarget - fCurPos) / (time * sr) : tableSize;
                state = iTarget == iCurPos ? Stopped :
                        (inc > 0.f ? Rising : Falling);
            }
        }

        void setPos(float pos) {
            fCurPos = std::fmax(0.f, std::fmin(posMax, pos * (float) tableSize));
            iCurPos =static_cast<int>(fCurPos);
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

        [[nodiscard]] float getCurrentValue() const {
            return table[iCurPos];
        }
    };
}

#endif
