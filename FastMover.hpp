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
#include <functional>

#include "clamp.hpp"

namespace dspkit {

    class FastMover {
    public:
        static constexpr int numTables = 31;
        static constexpr int tableSize = 4097;
        typedef const float (*TableData)[tableSize];

    private:
        static constexpr int tableSize_1 = tableSize - 1;
        static constexpr float posMax = static_cast<float>(tableSize_1);
        static const float defaultShapeTables[numTables][tableSize];
        TableData shapeTables;

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
#if 0
            float newPos = fCurPos + inc;
            fCurPos = newPos < posMax ? newPos : posMax;
#else
            fCurPos = inc_clamp_hi(fCurPos, inc, posMax);
#endif
            iCurPos = (int) fCurPos;
            moving = (iCurPos < tableSize); // <- FIXME: should be tablesize - 1 i guess
            curVal = start + scale * curTable[iCurPos];
        }

    public:
        explicit FastMover(TableData tableData = nullptr) {
            shapeTables = (tableData == nullptr) ? defaultShapeTables : tableData;
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

        void update() {
            if (moving) {
                move();
            }
        }

        float getNextValue() {
            update();
            return curVal;
        }

    public:
        // helper: initialize a set of table data using a transform function on the default tables.
        static void initTableData(float (*data)[tableSize], const std::function<float(float)> &transform) {
            for (int tab = 0; tab < numTables; ++tab) {
                for (int pos = 0; pos < tableSize; ++pos) {
                    data[tab][pos] = transform(defaultShapeTables[tab][pos]);
                }
            }
        }
    };
}

#endif //CRONE_FASTMOVER_HPP
