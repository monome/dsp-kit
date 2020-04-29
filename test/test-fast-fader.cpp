#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "FastFader.hpp"

dspkit::FastFader ff;
std::ofstream fs;

void beginOutput(std::string path) {
    fs.open(path);
    fs << "data = [ " << std::endl;
}

void process(int count) {
    for (int i=0; i<count; ++i) { 
	fs << ff.next() << ", ";
    }
}

void finishOutput() {
    fs << std::endl << " ]; " << std::endl;
    fs.close();
}



int main() {
    std::ofstream ofs;
    begin("test-fader-output.m");

    ff.setSampleRate(48000);
    ff.setTime(1.0);
    ff.setTarget(1.0);

    process(10000);

    ff.setTime(0.5);
    ff.setTarget(0.2;)

    process(10000);

    finishOutput();
}

