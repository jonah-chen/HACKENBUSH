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

add_executable(HACKENBUSH common/profile.hpp
        game/game.hpp
        game/generators.cpp
        game/generators.hpp
        game/nodes.cpp
        game/nodes.hpp
        game/prereqs.cpp
        game/prereqs.hpp
        interaction/input.cpp
        interaction/input.hpp
        render/buffer.cpp
        render/buffer.hpp
        render/camera.cpp
        render/camera.hpp
        render/geometry.cpp
        render/geometry.hpp
        render/shader.cpp
        render/shader.hpp
        game/game.cpp
        worldgen/parser.cpp worldgen/parser.hpp game/main.cxx)

add_executable(finite worldgen/finite.cxx)

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} ${OpenGlLinkers})
