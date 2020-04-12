
#include <random>
#include <sndfile.hh>
#include "DcBlocker.hpp"


std::default_random_engine randEngine;
std::uniform_real_distribution<float> randDist(0, 1);

dspkit::DcBlocker<float> dcBlocker;

static constexpr double twopi = 6.2831853071796;

const int sr = 48000.f;
static constexpr int numFrames = sr * 20;

// signal buffer
float sndBuf[numFrames];

static void fillSignalBufferNoise() {
    for (int fr = 0; fr < numFrames; ++fr) {
        sndBuf[fr] = randDist(randEngine) - 0.5;
    }
}



int main() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    SndfileHandle sndfile("test-dc-blocker.wav", SFM_WRITE, format, 1, sr);

    dcBlocker.init(sr, 60);
    fillSignalBufferNoise();
    for (int fr = 0; fr < numFrames; ++fr) {
	sndBuf[fr] = dcBlocker.processSample(sndBuf[fr]);
    }
    // write output soundfile
    sndfile.writef(sndBuf, numFrames);
}
