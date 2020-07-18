//
// Created by emb on 4/12/20.
//

#ifndef DSP_KIT_SMOOTHER_HPP
#define DSP_KIT_SMOOTHER_HPP

#include <limits>

#include "Envelope.hpp"
#include "Taper.hpp"


namespace dspkit {
    // smoother using a one-pole lowpass filter
    template<typename T>
    class OnePoleSmoother {
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
            if (t <= std::numeric_limits<float>::epsilon()) {
                y0 = x;
            }
        }

        // get new value without new input
        T getNextValue() {
            y0 = x0 + c * (y0 - x0);
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
    class AudioLevelSmoother {
    private:
        Envelope env;
        float time{};
        float target{};
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
#if 1
            return lookupLevel(env.processSample());
#else // testing...
            float pos = env.processSample();
            float amp = lookupLevel(pos);
            return amp;
#endif
        }

        // not used, but required by base template
        float getNextValue(float x) {
            setTarget(x);
            return getNextValue();
        }
    };


    // smoother which simply wraps an Envelope, without stage memory
    // FIXME: currently, Envelope only supports floats.
    class EnvelopeSmoother {
    private:
        Envelope env;
        float time{1};
        float target{0};
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
