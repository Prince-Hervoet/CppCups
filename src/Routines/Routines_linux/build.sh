#!/bin/bash
rm -r ./build
mkdir build
as -s simple_context.S -o simple_context.o
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..