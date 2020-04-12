//
// Created by emb on 4/12/20.
//

#ifndef DSP_KIT_SMOOTHER_HPP
#define DSP_KIT_SMOOTHER_HPP

namespace dspkit{

    // zero-cost abstraction for smoothing functions
    // (using Curiously Recurring Template Pattern)
    // @param Imp: implementation class
    // @param T: data type
    template<typename Imp, typname T>
    class Smoother {
    public:
        setSampleRate(T sr) {
            imp().setSampleRate(sr);
        }
        setTime(float time) {
            imp().setTime(time);
        }
        setTarget(T target) {
            imp().setTarget(target);
        }
        T getNextValue() {
            return imp().getNextValue();
        }
        T getNextValue(T x) {
            imp().setTarget(x);
            return imp().getNextValue();
        }

    private:
        Imp &imp() { return *static_cast<Imp*>(this); }
    };

    // smoother using a one-pole lowpass filter
    template <typename T>
    class OnePoleSmoother: public Smoother<OnePoleSmoother<T>, T> {

    };

    // smoother using an envelope and a table
    template <typename T>
    class TabSmoother: public Smoother<TabSmoother<T>, T> {

    };


};

#endif //DSP_KIT_SMOOTHER_HPP
