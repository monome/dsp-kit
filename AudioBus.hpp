//
// Created by emb on 4/12/20.
//

#ifndef DSP_KIT_AUDIOBUS_HPP
#define DSP_KIT_AUDIOBUS_HPP

#include "Smoother.hpp"
#include "Taper.hpp"

namespace dspkit {

    template<size_t numChannels, size_t blockSize>
    class AudioBus {
    protected:
        typedef AudioBus<numChannels, blockSize> BusT;
    public:
        float buf[numChannels][blockSize];

        // clear the entire bus
        void clear() {
            for (size_t ch = 0; ch < numChannels; ++ch) {
                for (size_t fr = 0; fr < blockSize; ++fr) {
                    buf[ch][fr] = 0.f;
                }
            }
        }

        // clear the first N frames in the bus
        void clear(size_t numFrames) {
            assert(numFrames < blockSize);
            for (size_t ch = 0; ch < numChannels; ++ch) {
                for (size_t fr = 0; fr < numFrames; ++fr) {
                    buf[ch][fr] = 0.f;
                }
            }
        }

        // copy from bus, with no scaling (overwrites previous contents)
        void copyFrom(AudioBus &b, size_t numFrames) {
            assert(numFrames < blockSize);
            for (size_t ch = 0; ch < numChannels; ++ch) {
                for (size_t fr = 0; fr < numFrames; ++fr) {
                    buf[ch][fr] = b.buf[ch][fr];
                }
            }
        }

        // copy from bus to pointer array, with no scaling (overwrites previous contents)
        void copyTo(float *dst[numChannels], size_t numFrames) {
            assert(numFrames < blockSize);
            for (size_t ch = 0; ch < numChannels; ++ch) {
                for (size_t fr = 0; fr < numFrames; ++fr) {
                    dst[ch][fr] = buf[ch][fr];
                }
            }
        }

        // sum from bus, without amplitude scaling
        void addFrom(BusT &b, size_t numFrames) {
            assert(numFrames < blockSize);
            for (size_t ch = 0; ch < numChannels; ++ch) {
                for (size_t fr = 0; fr < numFrames; ++fr) {
                    buf[ch][fr] += b.buf[ch][fr];
                }
            }
        }

        // mix from bus, with fixed amplitude
        void mixFrom(BusT &b, size_t numFrames, float level) {
            assert(numFrames < blockSize);
            for (size_t ch = 0; ch < numChannels; ++ch) {
                for (size_t fr = 0; fr < numFrames; ++fr) {
                    buf[ch][fr] += b.buf[ch][fr] * level;
                }
            }
        }


        // set from pointer array, without scaling
        void setFrom(const float *src[numChannels], size_t numFrames) {
            assert(numFrames < blockSize);
            for (size_t fr = 0; fr < numFrames; ++fr) {
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    buf[ch][fr] = src[ch][fr];
                }
            }
        }

        // mix from stereo bus with 2x2 level matrix
        void stereoMixFrom(BusT &b, size_t numFrames, const float level[4]) {
            assert(numFrames < blockSize);
            for (size_t fr = 0; fr < numFrames; ++fr) {
                buf[0][fr] += b.buf[0][fr] * level[0] + b.buf[1][fr] * level[2];
                buf[1][fr] += b.buf[0][fr] * level[1] + b.buf[1][fr] * level[3];
            }
        }


    };

    //-------------------------------------
    //-- audio bus with level smoothing

    template<int numChannels, size_t blockSize>
    class SmoothAudioBus : public AudioBus<numChannels, blockSize> {
    protected:
        typedef SmoothAudioBus<numChannels, blockSize> BusT;
    private:
        AudioLevelSmoother levelSmoother;
    public:
        void setLevel(float x) { levelSmoother.setTarget(x); }
        void setLevelSmoothTime(float x) { levelSmoother.setTime(x); }

        void mixFrom(BusT &b, size_t numFrames) {
            assert(numFrames < blockSize);
            float l;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                l = this->levelSmoother.getNextValue();
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    this->buf[ch][fr] += b.buf[ch][fr] * l;
                }
            }
        }

        // process gain smoothing, with no other changes
        void applyGain(size_t numFrames) {
            assert(numFrames < blockSize);
            float l;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                l = this->levelSmoother.getNextValue();
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    this->buf[ch][fr] *= l;
                }
            }
        }

        // mix from pointer array, with smoothed amplitude
        void mixFrom(const float *src[numChannels], size_t numFrames) {
            assert(numFrames < blockSize);
            float l;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                l = this->levelSmoother.getNextValue();
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    this->buf[ch][fr] += src[ch][fr] * l;
                }
            }
        }

        // set from pointer array, with smoothed amplitude
        void setFrom(const float *src[numChannels], size_t numFrames) {
            assert(numFrames < blockSize);
            float l;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                l = this->levelSmoother.getNextValue();
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    this->buf[ch][fr] = src[ch][fr] * l;
                }
            }
        }

        // mix from two busses with balance coefficient (linear)
        void xfade(BusT &a, BusT &b, size_t numFrames) {
            assert(numFrames < blockSize);
            float x, y, c;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                c = this->levelSmoother.getNextValue();
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    x = a.buf[ch][fr];
                    y = b.buf[ch][fr];
                    this->buf[ch][fr] = x + (y - x) * c;
                }
            }
        }

        // mix from two busses with balance coefficient (equal power)
        void xfadeEp(BusT &a, BusT &b, size_t numFrames) {
            assert(numFrames < blockSize);
            float x, y, l, c, d;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                l = this->levelSmoother.getNextValue() * (float) M_PI_2;
                c = sinf(l);
                d = cosf(l);
                for (size_t ch = 0; ch < numChannels; ++ch) {
                    x = a.buf[ch][fr];
                    y = b.buf[ch][fr];
                    this->buf[ch][fr] = x * c + y * d;
                }
            }
        }

    };

    template<size_t blockSize>
    class StereoSmoothAudioBus : public SmoothAudioBus<2, blockSize> {
    protected:
        typedef StereoSmoothAudioBus<blockSize> BusT;
    private:
        Envelope panSmoother;
        float pan;
        float smoothTime;
    public:
        StereoSmoothAudioBus() : pan() {

        }
        void setSampleRate(float sr) {
            this->levelSmoother.setSampleRate(sr);
            panSmoother.setSampleRate(sr);
        }
        void setPanPosition(float x) {
            pan = x;
            panSmoother.go(pan, smoothTime);
        }
        void setPanSmoothTime(float x) {
            smoothTime= x;
            panSmoother.go(pan, smoothTime);
        }

        // mix from mono->stereo bus, with level and pan (linear)
        void panMixFrom(AudioBus<1, blockSize> a, size_t numFrames) {
            assert(numFrames < blockSize);
            float l, c, x;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                x = a.buf[0][fr];
                l = this->levelSmoother.getNextValue();
                c = panSmoother.getNextValue();
                this->buf[0][fr] += x * l * (1.f - c);
                this->buf[1][fr] += x * l * c;
            }
        }


        // mix from mono->stereo bus, with level and pan (equal power)
        void panMixEpFrom(AudioBus<1, blockSize> a, size_t numFrames) {
            assert(numFrames < blockSize);
            float l, c, x;
            for (size_t fr = 0; fr < numFrames; ++fr) {
                x = a.buf[0][fr];
                l = this->levelSmoother.getNextValue();
                c = panSmoother.getNextValue();
                c *= (float) M_PI_2;
                this->buf[0][fr] += x * l * cosf(c);
                this->buf[1][fr] += x * l * sinf(c);
            }
        }

    };

}

#endif //DSP_KIT_AUDIOBUS_HPP
