#!/bin/bash
clang++ tests/test_gl.cxx interaction/input.cpp \
-I $(pwd) \
-I /usr/local/include/GLFW \
-L /usr/local/lib \
-lglfw3 -lpthread -lm -lz -lGL -lX11 -lXext -lXfixes -ldl -lGLEW \
-O3 -std=c++2a -fopenmp \
-o a.out && ./a.out && rm a.out