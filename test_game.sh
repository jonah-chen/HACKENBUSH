#!/bin/bash
clang++ -std=c++2a -O3 -fopenmp game/*.cpp game/test_game.cxx -o $1