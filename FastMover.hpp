//
// Created by emb on 4/28/20.
//
// FastMover is a fast, single-stage envelope
//
// it uses a set of static shape tables, and no interpolation
//
// operation is similar to FastFader, but each envelope segment is scaled
// to fit an arbitrary pair of points.

#ifndef CRONE_FASTMOVER_HPP
#define CRONE_FASTMOVER_HPP

#include <limits>
#include <cmath>

namespace dspkit {

    class FastMover {
    private:
        static constexpr int tableSize = 4097;
        static constexpr int tableSize_1 = tableSize - 1;
        static constexpr float posMax = static_cast<float>(tableSize_1);
        static constexpr int numTables = 31;
        static const float shapeTables[numTables][tableSize];

        float sr{};
        float time{};
        float inc{};

        // current segment definition
        float start{};
        float end{};
        float scale{};

        // current position as fractional table index
        float fCurPos{};
        int iCurPos{};
        float curVal{};

        const float *curTable{};
        const float *riseTable;
        const float *fallTable;

        bool moving{};

        void move() {
            float newPos = fCurPos + inc;
            fCurPos = newPos < posMax ? newPos : posMax;
            iCurPos = (int) fCurPos;
            moving = (iCurPos < tableSize);
            curVal = start + scale * curTable[iCurPos];
        }

    public:
        FastMover() {
            riseTable = shapeTables[0];
            fallTable = shapeTables[0];
        }
        void setSampleRate(float sr_) {
            sr = sr_;
        }

        void setTime(float t) {
            time = t;
            inc = (float) tableSize / (time * sr);
        }

        void setValue(float val) {
            curVal = val;
            moving = false;
        }

        void setTarget(float target) {
            if (time <= std::numeric_limits<float>::epsilon()) {
                setValue(target);
            } else {
                start = curVal;
                end = target;
                scale = end - start;
                curTable = end > start ? riseTable : fallTable;
                fCurPos = 0.f;
                iCurPos = 0;
                moving = true;
            }
        }

        void setRiseShape(int shapeTableIndex) {
            riseTable = shapeTables[shapeTableIndex];
        }

        void setFallShape(int shapeTableIndex) {
            fallTable = shapeTables[shapeTableIndex];
        }

        float getNextValue() {
            if (moving) { move(); }
            return curVal;
        }

    };
}

#endif //CRONE_FASTMOVER_HPP
