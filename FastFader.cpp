#include "FastFader.hpp"

using dspkit::FastFader;
const float FastFader::table[FastFader::tableSize] = {
    #include "inc/audio_taper_data.inc"
};
