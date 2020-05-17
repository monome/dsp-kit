#include <string>
#include <iostream>
#include <fstream>
#include <easing.hpp>

#include <chrono>

#include "FastMover.hpp"
using namespace dspkit;

#define SKIP_OUTPUT true

FastMover   fm;
std::ofstream fs;

constexpr unsigned long int nsamps = 200000;
std::array<float, nsamps> buf;
unsigned long int count = 0;

void beginOutput(const std::string &path) {
    count = 0;
#if SKIP_OUTPUT
#else
    fs.open(path);
    fs << path << std::endl;
#endif
}

void process(int n) {
    float * p = &(buf[count]);
    for (int i=0; i<n; ++i) {
        *p++ = fm.getNextValue();
    }
    count += n;
}

void finishOutput() {
#if SKIP_OUTPUT
#else
    for (unsigned long int i=0; i<count; ++i) {
        fs << buf[i]<< std::endl;
    }
    fs.close();
#endif
}

void testShape(int shapeIndex) {
    using namespace std::chrono;

    std::string shapeName = easing::function_info[shapeIndex];
    std::string outputFileName = "test_" + shapeName + ".data";
    beginOutput(outputFileName);

    auto start = high_resolution_clock::now();


    fm.setRiseShape(shapeIndex);
    fm.setFallShape(shapeIndex);
    fm.setSampleRate(48000);

    // rise
    fm.setTime(0);
    fm.setTarget(0);
    fm.setTime(0.5);
    fm.setTarget(100.0);
    process(24000);
    // fall
    fm.setTarget(0);
    process(24000);
    // immediate jump
    fm.setTime(0.0);
    fm.setTarget(-100);
    // aborted rise
    fm.setTime(1.0);
    fm.setTarget(0.0);
    process(24000);
    // aborted fall
    fm.setTime(1.0);
    fm.setTarget(-100.0);
    process(24000);

    auto end = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(end - start);
    std::cout << dur.count() << std::endl;

    finishOutput();
}


int main() {
    for (int i=0; i<(int)easing::function::enum_count; ++i) {
        testShape(i);
    }
}

