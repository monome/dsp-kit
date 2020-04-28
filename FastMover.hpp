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
    static constexpr int tableSize = 1025;
    static constexpr float posMax = static_cast<float>(tableSize) - std::numeric_limits<float>::epsilon() ;
    static constexpr int numTables = 4;

    static const float shapeTables[tableSize][numTables];

    float sr;
    float time;
    float inc;

    // current segment definition
    float start;
    float end;
    float scale;

    // current position as fractional table index
    float fCurPos;
    int iCurPos;
    float curVal;

    const float *curTable;
    const float *riseTable;
    const float *fallTable;

    bool moving;

    void move() {
        fCurPos = std::fmin(posMax, fCurPos + inc);
        iCurPos = (int)fCurPos;
    }
public:
    void setSampleRate(float sr_) {
        sr = sr_;
    }

    void setTime(float t) {
        time = t;
        inc = (float)tableSize / (time * sr);
    }

    void setTarget(float target) {
        start = curVal;
        end = target;
        scale = end - start;
        curTable = end > start ? riseTable : fallTable;
    }

    void setRiseShape(int shapeTableIndex) {
        riseTable = shapeTables[shapeTableIndex];
    }

    void setFallShape(int shapeTableIndex) {
        fallTable = shapeTables[shapeTableIndex];
    }

    float next() {
        if (moving) { move(); }
        curVal = start + scale * curTable[iCurPos];
    }};

}

#endif //CRONE_FASTMOVER_HPP
