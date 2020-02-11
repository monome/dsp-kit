
#include <chrono>
#include <random>
#include <iostream>

#include <sndfile.hh>

#include "Svf.hpp"

const int sr = 48000.f;
dspkit::Svf svf;

std::default_random_engine randEngine;
std::uniform_real_distribution<float> randDist(0, 1);

const float twopi = 6.2831853071796;

int main() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    SndfileHandle sndfile("test-svf.wav", SFM_WRITE, format, 1, sr);

    svf.initPitchTable(sr);

    svf.setSr(sr);
    svf.setFc(1000);
    svf.setRq(0.5);

    svf.setLpMix(1.f);
    svf.setHpMix(0.f);
    svf.setBpMix(0.f);
    svf.setBrMix(0.f);

    int numFrames = sr * 4;
    float modInc = 4.0 / sr;
    float modPhase = 0;
    float mod = 0;

    // store modulated frequency/pitch in own buffer for good profiling comparison
    float freqBuf[numFrames];
    float pitchBuf[numFrames];

    for (int fr = 0; fr < numFrames; ++fr) {
        modPhase += modInc;
        while (modPhase > twopi) { modPhase -= twopi; }
        mod = sinf(modPhase) * 0.5f + 0.5f;
        pitchBuf[fr] = mod;
        // these magic numbers are from Svf pitch table setup,
        // with default values for base freq and octave count
        freqBuf[fr] = 14.1 * pow(2, mod * 10.0);
    }

    // signal buffer
    float sndBuf[numFrames];

    // fill with noise
    for (int fr = 0; fr < numFrames; ++fr) {
        sndBuf[fr] = randDist(randEngine) - 0.5;
    }

    // test execution speed with pitch control (using primary coefficient LUT)
    auto start = std::chrono::high_resolution_clock::now();
    for (int fr = 0; fr < numFrames; ++fr) {
        svf.setFcPitch(pitchBuf[fr]);
        sndBuf[fr] = svf.processSample(sndBuf[fr]);
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "pitch modulation: processed " << numFrames << " samples"
              << " in " << microseconds << " usecs" << std::endl;


    start = std::chrono::high_resolution_clock::now();
    // test execution speed with frequency control (compute primary coeff)
    for (int fr = 0; fr < numFrames; ++fr) {
        svf.setFc(freqBuf[fr]);
        sndBuf[fr] = svf.processSample(sndBuf[fr]);
    }
    elapsed = std::chrono::high_resolution_clock::now() - start;
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "freq modulation: processed " << numFrames << " samples"
              << " in " << microseconds << " usecs" << std::endl;

    // write output soundfile
    for (int fr = 0; fr < numFrames; ++fr) {
        sndfile.writef(sndBuf + fr, 1);
    }
}
