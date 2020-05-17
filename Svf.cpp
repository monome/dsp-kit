#include <cmath>
//#include <iostream>

#include "Lut.hpp"
#include "Svf.hpp"
#include "Constants.hpp"
#include "Conversion.hpp"

using dspkit::Svf;
using dspkit::Lut;
using dspkit::Constants;

Svf::Svf() {
    clear();
}

// pitch table maps [0, 1] to midi [0, 127]
void Svf::initPitchTable(double sr_) {
    auto sz_1 = gTabSize -1;
    double x = 0.0;
    double inc = 127.0 / sz_1;
    for (unsigned int i = 0; i < sz_1; ++i) {
        x += inc;
        double hz = Conversion<double>::midihz(x);
        gTab[i] = static_cast<float>(tan(M_PI * hz / sr_));
    }
    // extra element for "extended" lookup
    gTab[sz_1] = gTab[sz_1 - 1];
}

void Svf::setCutoffPitch(float pitch) {
    this->g = Lut<float>::lookupLinear(pitch, gTab.data(), gTabSize);
    calcSecondaryCoeffs();
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
    this->sr_2 = sr * 0.5f;
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
    g2 = 2.f * (g + rq) * g1;
    g3 = g * g1;
    g4 = 2.f * g1;
}

float Svf::getG(float sr, float fc) {
    return static_cast<float>(tan(Constants::pi * fc / sr));
}