#ifndef DSPKIT_DC_BLOCKER_HPP
#define DSPKIT_DC_BLOCKER_HPP
// 1-pole, 1-zero highpass, suitable for cheap DC blocking

namespace dspkit {
    template<typename T>
    class DcBlocker {
        T c; // coefficient (pole position)
        T x1; // input history
        T y1; // output history

    public:
        // initialize with sample rate and -3db break frequency
        void init(T sr, float fb = 30) {
            static constexpr double pi = 3.1415926535898;
            double wn = pi * fb / sr;
            double b0 = 1.0 / (1.0 + wn);
            c = (1 - wn) * b0;
            x1 = 0;
            y1 = 0;
        }

        // process a single sample
        T processSample(T x) {
            T y = x - x1 + c * y1;
            x1 = x;
            y1 = y;
            return y;
        }
    };
}

#endif // DSPKIT_DC_BLOCKER_HPP