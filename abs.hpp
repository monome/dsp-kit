//
// Created by emb on 6/7/20.
//

#ifndef DSP_KIT_ABS_H
#define DSP_KIT_ABS_H

namespace dspkit {
    // this is actually the fastest `abs` i could find.
    template <typename T>
    static T abs(T x) {
        return x > 0 ? x : -x;
    }
}

#endif //DSP_KIT_ABS_H
