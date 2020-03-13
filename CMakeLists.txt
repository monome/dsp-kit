cmake_minimum_required(VERSION 3.7)
project(dsp_kit)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_subdirectory(lib/cnpy)

add_executable(test-fifo
        test/test-fifo.cpp
        Fifo.hpp)

add_executable(test-smoothing
        test/test-smoothing.cpp
        Smoothing.hpp)

add_executable(test-envelope
        test/test-envelope.cpp
        Envelope.cpp
        Envelope.hpp)

add_executable(test-svf
        test/test-svf.cpp
        Svf.cpp
        Svf.hpp)      

target_link_libraries(test-envelope cnpy)

target_link_libraries(test-svf sndfile)

add_subdirectory(noise)
