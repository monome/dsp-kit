//
// Created by emb on 5/17/20.
//

#ifndef DSPKIT_CLAMP_HPP
#define DSPKIT_CLAMP_HPP

namespace dspkit {
    template<typename T>
    static inline T clamp(T x, T min, T max) {
        return x < min ? min : (x > max ? max : x);
    }

    template<typename T>
    static inline T clamp_hi(T x, T max) {
        return x > max ? max : x;
    }


    template<typename T>
    static inline T clamp_lo(T x, T min) {
        return x < min ? min : x;
    }

    template<typename T>
    static inline T inc_clamp(T x, T inc, T min, T max) {
        T y = x + inc;
        return y < min ? min : (y > max ? max : y);
    }

    template<typename T>
    static inline T inc_clamp_hi(T x, T inc, T max) {
        T y = x + inc;
        return y > max ? max : y;
    }

    template<typename T>
    static inline T inc_clamp_lo(T x, T inc, T min) {
        T y = x + inc;
        return y < min ? min : y;
    }

}
#endif //DSPKIT_CLAMP_HPP
