cmake_minimum_required(VERSION 3.16)
project(HACKENBUSH)

set(CMAKE_CXX_STANDARD 20)

# Use openmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fopenmp")
set(OpenGlLinkers -lglfw3 -lpthread -lm -lz -lGL -lX11 -lXext -lXfixes -ldl -lGLEW)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS})

include_directories(.)

add_executable(HACKENBUSH tests/test_prototype.cxx
        common/profile.hpp
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
        game/client.hpp
        game/client.cpp)

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} ${OpenGlLinkers})
