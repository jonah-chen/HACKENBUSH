#!/bin/bash
clang++ game/*.cpp tests/test_game.cxx \
-I $(pwd) \
-std=c++2a -O3 -fopenmp \
-o a.out && ./a.out && rm a.out