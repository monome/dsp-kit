cmake_minimum_required(VERSION 3.15)
project(dsp_kit)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(fifo-test
        fifo-test.cpp
        fifo.hpp)

add_executable(smoothing-test
        smoothing-test.cpp
        Smoothing.hpp)

add_executable(envelope-test
        envelope-test.cpp
        Envelope.cpp
        Envelope.hpp)
