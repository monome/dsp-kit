//
// Created by emb on 11/19/18.
//

#ifndef DSPKIT_BUS_H
#define DSPKIT_BUS_H

#include "dsp-kit/Smoother.hpp"
#include "dsp-kit/FastFader.hpp"
#include "dsp-kit/FastMover.hpp"

namespace dspkit {

    template<size_t numChannels, size_t blockSize>
    class AudioBus {
    private:
        typedef AudioBus<numChannels, blockSize> BusT;
    public:
        float buf[numChannels][blockSize];
        typedef dspkit::FastFader LevelSmoother;
        typedef dspkit::FastMover PanSmoother;

        // clear the entire bus
        void clear() {
            for(size_t ch=0; ch < numChannels; ++ch) {
                for(size_t fr=0; fr < blockSize; ++fr) {
                    buf[ch][fr] = 0.f;
                }
            }
        }

        // clear the first N frames in the bus
        void clear(size_t numFrames) {
            assert(numFrames < blockSize);
            for(size_t ch=0; ch < numChannels; ++ch) {
                for(size_t fr=0; fr<numFrames; ++fr) {
                    buf[ch][fr] = 0.f;
                }
            }
        }

        // copy from bus, with no scaling (overwrites previous contents)
        void copyFrom(AudioBus &b, size_t numFrames) {
            assert(numFrames < blockSize);
            for(size_t ch=0; ch < numChannels; ++ch) {
                for(size_t fr=0; fr<numFrames; ++fr) {
                    buf[ch][fr] = b.buf[ch][fr];
                }
            }
        }

        // copy from bus to pointer array, with no scaling (overwrites previous contents)
        void copyTo(float *dst[numChannels], size_t numFrames) {
            assert(numFrames < blockSize);
            for(size_t ch=0; ch < numChannels; ++ch) {
                for(size_t fr=0; fr<numFrames; ++fr) {
                    dst[ch][fr] = buf[ch][fr];
                }
            }
        }


        // sum from bus, without amplitude scaling
        void addFrom(BusT &b, size_t numFrames) {
            assert(numFrames < blockSize);
            for(size_t ch=0; ch < numChannels; ++ch) {
                for(size_t fr=0; fr<numFrames; ++fr) {
                    buf[ch][fr] += b.buf[ch][fr];
                }
            }
        }

        // mix from bus, with fixed amplitude
        void mixFrom(BusT &b, size_t numFrames, float level) {
            assert(numFrames < blockSize);
            for(size_t ch=0; ch < numChannels; ++ch) {
                for(size_t fr=0; fr<numFrames; ++fr) {
                    buf[ch][fr] += b.buf[ch][fr] * level;
                }
            }
        }


        // mix from bus, with smoothed amplitude
        void mixFrom(BusT &b, size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float l;
            for(size_t fr=0; fr<numFrames; ++fr) {
                l = level.getNextValue();
                for(size_t ch=0; ch < numChannels; ++ch) {
                    buf[ch][fr] += b.buf[ch][fr] * l;
                }
            }
        }

        // apply smoothed amplitude
        void applyGain(size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float l;
            for(size_t fr=0; fr<numFrames; ++fr) {
                l = level.getNextValue();
                for(size_t ch=0; ch < numChannels; ++ch) {
                    buf[ch][fr] *= l;
                }
            }
        }

        // mix from pointer array, with smoothed amplitude
        void mixFrom(const float *src[numChannels], size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float l;
            for(size_t fr=0; fr<numFrames; ++fr) {
                l = level.getNextValue();
                for(size_t ch=0; ch < numChannels; ++ch) {
                    buf[ch][fr] += src[ch][fr] * l;
                }
            }
        }

        // set from pointer array, with smoothed amplitude
        void setFrom(const float *src[numChannels], size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float l;
            for(size_t fr=0; fr<numFrames; ++fr) {
                l = level.getNextValue();
                for(size_t ch=0; ch < numChannels; ++ch) {
                    buf[ch][fr] = src[ch][fr] * l;
                }
            }
        }

        // set from pointer array, without scaling
        void setFrom(const float *src[numChannels], size_t numFrames) {
            assert(numFrames < blockSize);
            for(size_t fr=0; fr<numFrames; ++fr) {
                for(size_t ch=0; ch < numChannels; ++ch) {
                    buf[ch][fr] = src[ch][fr];
                }
            }
        }

        // mix to pointer array, with smoothed amplitude
        void mixTo(float *dst[numChannels], size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float l;
            for(size_t fr=0; fr<numFrames; ++fr) {
                l = level.getNextValue();
                for(size_t ch=0; ch < numChannels; ++ch) {
                    dst[ch][fr] = buf[ch][fr] * l;
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

        // mix from two busses with balance coefficient (linear)
        void xfade(BusT &a, BusT &b, size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float x, y, c;
            for(size_t fr=0; fr<numFrames; ++fr) {
                c = level.getNextValue();
                for(size_t ch=0; ch < numChannels; ++ch) {
                    x = a.buf[ch][fr];
                    y = b.buf[ch][fr];
                    buf[ch][fr] = x + (y-x) * c;
                }
            }
        }

        // mix from two busses with balance coefficient (equal power)
        void xfadeEp(BusT &a, BusT &b, size_t numFrames, LevelSmoother &level) {
            assert(numFrames < blockSize);
            float x, y, l, c, d;
            for(size_t fr=0; fr<numFrames; ++fr) {
                l = level.getNextValue() * (float)M_PI_2;
                c = sinf(l);
                d = cosf(l);
                for(size_t ch=0; ch < numChannels; ++ch) {
                    x = a.buf[ch][fr];
                    y = b.buf[ch][fr];
                    buf[ch][fr] = x * c + y * d;
                }
            }
        }

        // mix from mono->stereo bus, with level and pan (linear)
        void panMixFrom(AudioBus<1, blockSize> a, size_t numFrames,
                        LevelSmoother &level, PanSmoother& pan) {
            assert(numFrames < blockSize);
            static_assert(numChannels > 1, "using panMixFrom() on mono bus");
            float l, c, x;
            for(size_t fr=0; fr<numFrames; ++fr) {
                x = a.buf[0][fr];
                l = level.getNextValue();
                c = pan.getNextValue();
                buf[0][fr] += x*l*(1.f-c);
                buf[1][fr] += x*l*c;
            }
        }


        // mix from mono->stereo bus, with level and pan (equal power)
        void panMixEpFrom(AudioBus<1, blockSize> a, size_t numFrames,
                LevelSmoother &level,  PanSmoother& pan) {
            assert(numFrames < blockSize);
            static_assert(numChannels > 1, "using panMixFrom() on mono bus");
            float l, c, x;
            for(size_t fr=0; fr<numFrames; ++fr) {
                x = a.buf[0][fr];
                l = level.getNextValue();
                c = pan.getNextValue();
                c *= (float)M_PI_2;
                buf[0][fr] += x*l * cosf(c);
                buf[1][fr] += x*l * sinf(c);
            }
        }
    };

}

#endif //DSPKIT_BUS_H
