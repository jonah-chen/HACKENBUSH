#!/bin/bash
clang++ tests/test_"$1".cxx interaction/input.cpp render/*.cpp game/*.cpp \
-I $(pwd) \
-I /usr/local/include/GLFW \
-L /usr/local/lib \
-lglfw3 -lpthread -lm -lz -lGL -lX11 -lXext -lXfixes -ldl -lGLEW \
-O3 -std=c++2a -fopenmp \
-o a.out && ./a.out && rm a.out