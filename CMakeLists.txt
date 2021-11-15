cmake_minimum_required(VERSION 3.16)
project(HACKENBUSH)

set(CMAKE_CXX_STANDARD 20)

# Use openmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fopenmp")
set(OpenGlLinkers -lglfw3 -lpthread -lm -lz -lGL -lX11 -lXext -lXfixes -ldl -lGLEW)
set(CMAKE_BUILD_TYPE Release)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS})

include_directories(.)

add_executable(HACKENBUSH game/main.cxx
        game/game.cpp
        game/nodes.cpp
        game/prereqs.cpp
        game/generators.cpp
        interaction/input.cpp
        render/buffer.cpp
        render/camera.cpp
        render/geometry.cpp
        render/shader.cpp
        worldgen/parser.cpp)

add_executable(finite worldgen/finite.cxx)

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} ${OpenGlLinkers})
