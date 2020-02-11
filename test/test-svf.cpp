#include <sndfile.hh>
#include <cmath>

#include "Svf.hpp"

const int sr = 48000.f;
dspkit::Svf svf;


const float twopi = 6.2831853071796;
int main() {
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    SndfileHandle sndfile("test-svf.wav", SFM_WRITE, format, 1, sr);

    svf.initPitchTable(sr);

    int numFrames = sr * 4;
    float modInc = 1.0 / sr;
    float modPhase = 0;
    float mod = 0.;
    
    for (int fr=0; fr<numFrames; ++fr)  {
	// update modulator
	modPhase += modInc;
	while (modPhase > twopi) { modPhase -= twopi; }
	mod = sinf(modPhase) * 0.5f + 0.5f;
	// apply modulator
	svf.setFcPitch(mod);
	// generate noise
	
	// filter noise
    }
}
