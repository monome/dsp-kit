#include <sndfile.hh>
#include <random>

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
    svf.setRq(0.5);

    int numFrames = sr * 4;
    float modInc = 1.0 / sr;
    float modPhase = 0;
    float mod = 0;
    
    for (int fr=0; fr<numFrames; ++fr)  {
	// update modulator
	modPhase += modInc;
	while (modPhase > twopi) { modPhase -= twopi; }
	mod = sinf(modPhase) * 0.5f + 0.5f;
	// apply modulator
	svf.setFcPitch(mod);
	float noise = randDist(randEngine);
	float filtered = svf.processSample(noise);
	sndfile.writef(&filtered, 1);
    }
}