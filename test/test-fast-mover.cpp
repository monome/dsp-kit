#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <easing.hpp>

#include "FastMover.hpp"

dspkit::FastMover fm;
std::ofstream fs;

void beginOutput(const std::string &path) {
    fs.open(path);
    //fs << "data = [ " << std::endl;
}

void process(int count) {
    for (int i=0; i<count; ++i) {
        fs << fm.next() << std::endl;
    }
}

void finishOutput() {
    //fs << std::endl << " ]; " << std::endl;
    fs.close();
}

void testShape(int shapeIndex) {
    std::string shapeName = easing::function_info[shapeIndex];
    std::string outputFileName = "test_" + shapeName + ".data";
    beginOutput(outputFileName);
    fm.setRiseShape(shapeIndex);
    fm.setFallShape(shapeIndex);
    fm.setSampleRate(48000);
    fm.setTime(0);
    fm.setTarget(50);
    fm.setTime(1.0);
    fm.setTarget(0.0);
    process(20000);

    fm.setTime(0.5);
    fm.setTarget(100);

    process(20000);

    fm.setTime(0.3);
    fm.setTarget(-28);

    process(20000);

    finishOutput();

}


int main() {
    testShape(0);
    testShape(1);
    testShape(2);
}

