#!/bin/bash
clang++ tests/test_geometry.cxx \
-I $(pwd) \
-std=c++2a -O3 -fopenmp \
-o test_geometry.out