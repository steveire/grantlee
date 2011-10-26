#!/bin/bash

rm -rf coverage_build
mkdir coverage_build
cd coverage_build
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/modules/TestCocoonToolchain.cmake -DCMAKE_BUILD_TYPE=TestCocoon
make
make testcocoon

