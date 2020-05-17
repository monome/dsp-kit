//
// Created by emb on 5/16/20.
//

#ifndef CRONE_CONVERSION_HPP
#define CRONE_CONVERSION_HPP

namespace dspkit {
    template<typename T>
    class Conversion {
    public:
        static T midihz(T midi) {
            return 440 * powf(2.0, (midi-69)/12);
        }
        static T hzmidi(T hz) {
            return 69 + (log2(hz/440)*12);
        }
        static T ampdb(T amp) {
            return log10(amp) * 20;
        }
        static T dbamp(T db) {
            return powf(10.0, db * 0.05);
        }
    };
}

#endif //CRONE_CONVERSION_HPP
