#include <cmath>
#include <boost/assert.hpp>
//#include <iostream>

#include "Lut.hpp"
#include "Svf.hpp"
#include "Constants.hpp"
#include "Conversion.hpp"

using dspkit::Svf;
using dspkit::Lut;
using dspkit::Constants;

Svf::Svf() : gTable(nullptr) {
    clear();
}

void Svf::clear() {
    v0z = 0;
    v1 = 0;
    v2 = 0;
}

void Svf::calcCoeffs() {
    g = getG(sr, fc);
    calcSecondaryCoeffs();
}

float Svf::processSample(float in) {
    update(in);
    return lp*lpMix + bp*bpMix + hp*hpMix + br*brMix;
}

void Svf::update(float in) {
    // update
    v0 = in;
    v1z = v1;
    v2z = v2;
    v3 = v0 + v0z - 2.f * v2z;
    v1 += g1 * v3 - g2 * v1z;
    v2 += g3 * v3 + g4 * v1z;
    v0z = v0;
    // output
    lp = v2;
    bp = v1;
    hp = v0 - rq * v1 - v2;
    br = v0 - rq * v1;
}


void Svf::setSampleRate(float sr_) {
    this->sr = sr_;
    calcCoeffs();
}

void Svf::setCutoffNoCalc(float fc_) {
    this->fc = fc_;
}

void Svf::setInverseQNoCalc(float rq_) {
    this->rq = rq_;
}

void Svf::setCutoff(float fc_) {
    setCutoffNoCalc(fc_);
    calcCoeffs();
}

void Svf::setInverseQ(float rq_) {
    setInverseQNoCalc(rq_);
    calcSecondaryCoeffs();
}

void Svf::setLpMix(float mix) {
    lpMix = mix;
}

void Svf::setHpMix(float mix) {
    hpMix = mix;
}

void Svf::setBpMix(float mix) {
    bpMix = mix;
}

void Svf::setBrMix(float mix) {
    brMix = mix;
}

// calculate only secondary coefficients, given primary (e.g. from LUT)
void Svf::calcSecondaryCoeffs() {
    g1 = g / (1.f + g * (g + rq));
    g3 = g * g1;
    g4 = 2.f * g1;
    g2 = (g + rq) * g4;
}

float Svf::getG(float sr, float fc) {
    return static_cast<float>(tan(Constants::pi * fc / sr));
}

void Svf::setG(float x) {
    this->g = x;
}

void Svf::setGTable(const float *gainTable, int tableSize) {
    this->gTable = gainTable;
    gTableSize = tableSize;
}

void Svf::fillGTable(float* table, int size, float sampleRate, float midiMin, float midiMax) {
    double x = midiMin;
    double inc = 1.0 / size;
    for (int pos=0; pos<size; ++pos) {
        double midi = x * (midiMax - midiMin) + midiMin;
        double hz = Conversion<double>::midihz(midi);
        table[pos] = Svf::getG((float) sampleRate, (float) hz);
        x += inc;
    }
}

// assumption: gain table is not null
void Svf::setCutoffPitchNoCalc(float pitch) {
    assert(this->gTable != nullptr);
    setG(Lut<float>::lookupLinear(pitch, gTable, gTableSize));
}
