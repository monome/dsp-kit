//
// Created by emb on 4/28/20.
//
#include <iostream>
#include <fstream>

#include "easing.hpp"

void genTable(int n, int shape) {
    auto fn = easing::get<double>(static_cast<easing::function>(shape));
    auto name = easing::function_info[shape];

    double x = 0;
    double inc = 1.0 / (n-1);

    std::ofstream ofs("easing_" + name + std::string(".inc"));

    for (int i=0; i<n; ++i) {
        //std::cout << fn(x) << std::endl;
        ofs << fn(x) << ", ";
        x += inc;
    }
    ofs.close();
}


void genTableSvfFc(int n, int shape) {
    auto fn = easing::get<double>(static_cast<easing::function>(shape));
    auto name = easing::function_info[shape];

    double x = 0;
    double inc = 1.0 / (n-1);

    std::ofstream ofs("easing_" + name + std::string(".inc"));

    for (int i=0; i<n; ++i) {
        //std::cout << fn(x) << std::endl;
        ofs << fn(x) << ", ";
        x += inc;
    }
    ofs.close();
}

int main() {

    // table size
    int n = 4097;

    int numShapes = (int)easing::function::enum_count;

    for (int shape=0; shape<numShapes; ++shape) {
        genTable(n, shape);
    }



}
