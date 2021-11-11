
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "profile.hpp"
#include "interaction/input.hpp"
int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Use opengl 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    user_inputs prev_inputs, cur_inputs;

    
    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // set the initial mouse position
    glfwGetCursorPos(window, &XPOS(prev_inputs), &YPOS(prev_inputs));

    prev_inputs = fetch_inputs(window);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
    PROFILE_START;
        cur_inputs = fetch_inputs(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        std::cout << cur_inputs - prev_inputs << std::endl;
        prev_inputs = cur_inputs;

    PROFILE_LOG;

        // swap buffers
        glfwSwapBuffers(window);// this is what takes up the rest of the time
                                // to maintain 60fps.
        // poll events
        glfwPollEvents();
    }

    // terminate
    glfwTerminate();
    return 0;
}