#!/bin/bash
clang++ game/*.cpp game/test_game.cxx \
-I /home/hina/Code/HACKENBUSH \
-std=c++2a -O3 -fopenmp \
-o $1