//
// Created by emb on 4/12/20.
//

#ifndef DSP_KIT_SMOOTHER_HPP
#define DSP_KIT_SMOOTHER_HPP

#include "Envelope.hpp"
#include "Taper.hpp"

namespace dspkit {

    // zero-cost abstraction for smoothing functions
    // (using Curiously Recurring Template Pattern)
    // @param Imp: implementation class
    // @param T: data type
    template<typename Imp, typename T>
    class Smoother {
    public:
        void setSampleRate(T sr) {
            imp().setSampleRate(sr);
        }
        void setTime(float time) {
            imp().setTime(time);
        }
        void setTarget(T target) {
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
        T sr;
            T c;
            T x0;
            T y0;
            T t;

        private:
            void calcCoeff() {
                c = expf(-6.9f / (t * sr));
            }

        public:
            void init(T samplerate, T time) {
                sr = samplerate;
                t = time;
                calcCoeff();
            }

            void setSampleRate(T samplerate) {
                sr = samplerate;
                calcCoeff();
            }

            void setTime(T time) {
                t = time;
                calcCoeff();
            }

            void setTarget(T x) {
                x0 = x;
            }

            // get new value without new input
            T getNextValue() {
                y0 = smooth(x0, y0, c);
                return y0;
            }

            // get new value with input
            T getNextValue(T x) {
                setTarget(x);
                return getNextValue();
            }

            OnePoleSmoother() {
                x0 = 0.f;
                y0 = 0.f;
                c = 0.f;
                sr = 1.f;
                t = 0.f;
            }

    };


    // smoother using an envelope and an audio taper
    // note that the target values are given in terms of taper position:
    // 0    -> 0
    // 0.3  -> -30db
    // 0.5  -> -20db
    // 0.75 -> -10db
    // 1   - > 1
    class AudioLevelSmoother: public Smoother<AudioLevelSmoother, float> {
    private:
        Envelope env;
        float time;
        float target;
    private:
        static float lookupLevel(float pos) {
            return Taper::LevelControl::getAmp(pos);
        }
    public:
        void setSampleRate(float sr) {
            env.setSampleRate(sr);
        }
        void setTime(float t) {
            time = t;
            env.go(target, time);
        }
        void setTarget(float val) {
            target = val;
            env.go(target, time);
        }
        float getNextValue() {
            return lookupLevel(env.processSample());
        }

        // not used, but required by base template
        float getNextValue(float x) {
            setTarget(x);
            return getNextValue();
        }
    };


    // smoother which simply wraps an Envelope, without stage memory
    // FIXME: currently, Envelope only supports floats.
    class EnvelopeSmoother: public Smoother<EnvelopeSmoother, float> {
    private:
        Envelope env;
        float time;
        float target;
    public:
        void setSampleRate(float sr) {
            env.setSampleRate(sr);
        }
        void setTime(float t) {
            time = t;
            env.go(target, time);
        }
        void setTarget(float val) {
            target = val;
            env.go(target, time);
        }

        float getNextValue() {
            return env.processSample();
        }

        float getNextValue(float x) {
            setTarget(x);
            return getNextValue();
        }

        void setRiseShape(easing::function shape) {
            env.setRiseShape(shape);
        }

        void setFallShape(easing::function shape) {
            env.setFallShape(shape);
        }

    };


    // smoother using an envelope and a generic table
    // TODO

}

#endif //DSP_KIT_SMOOTHER_HPP
