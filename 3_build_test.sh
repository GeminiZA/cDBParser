#!/bin/bash

mkdir -p build
cd build
cmake ..
make

./tests/parser_test
