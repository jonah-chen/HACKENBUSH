#!/bin/bash
clang++ game/*.cpp render/test_gl.cxx \
-I /usr/local/include/GLFW -I /home/hina/Code/HACKENBUSH \
-L /usr/local/lib \
-lglfw3 -lpthread -lm -lz -lGL -lX11 -lXext -lXfixes -ldl -lGLEW \
-O3 -std=c++2a -fopenmp \
-o $1