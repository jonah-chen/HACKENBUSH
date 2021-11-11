#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

#define  XPOS(IN) ( (IN.fp)[0] )
#define  YPOS(IN) ( (IN.fp)[1] )
#define   LMB(IN) ( (IN.tf)[0] )
#define   RMB(IN) ( (IN.tf)[1] )
#define   K_W(IN) ( (IN.tf)[2] )
#define   K_A(IN) ( (IN.tf)[3] )
#define   K_S(IN) ( (IN.tf)[4] )
#define   K_D(IN) ( (IN.tf)[5] )
#define K_ESC(IN) ( (IN.tf)[6] )

#define FLOATPOINT_INPUTS 2
#define TRUE_FALSE_INPUTS 7

struct user_inputs
{
    double fp[FLOATPOINT_INPUTS];
    bool   tf[TRUE_FALSE_INPUTS];

    user_inputs() = default;
    user_inputs(const user_inputs&) = default;
    user_inputs& operator=(const user_inputs&) = default;

    /** 
     * @param rhs usually the previous inputs
     * @return the difference between the two inputs. meaning:
     * - the "displacement" of the cursor (divide by frame time yields velocity)
     * - the "down event" of the buttons (true if the button was pressed down 
     *   exactly during the current frame, false otherwise)
     */
    user_inputs operator-(const user_inputs &rhs)
    {
        user_inputs ret;
        for (int i = 0; i < FLOATPOINT_INPUTS; ++i)
            ret.fp[i] = fp[i] - rhs.fp[i];
        for (int i = 0; i < TRUE_FALSE_INPUTS; ++i)
            ret.tf[i] = tf[i] != rhs.tf[i];

        return ret;
    }

    friend std::ostream& operator<<(std::ostream& os, const user_inputs& ui)
    {
        os << "xpos:" << XPOS(ui) << " ypos:" << YPOS(ui) 
           << " lmb:" << LMB(ui) << " rmb:" << RMB(ui) 
           << " W:"   << K_W(ui) << " A:" << K_A(ui) 
           << " S:" << K_S(ui) << " D:" << K_D(ui) << " ESC:" << K_ESC(ui);
        return os;
    }
};

/**
 * @brief Fetch the user inputs from the GLFW window and store them in the
 * user_inputs struct.
 * 
 * @param window GLFWwindow pointer to the window to fetch the inputs from
 * @return struct of the user inputs consisting of the mouse position, relavent 
 * keys, and mouse buttons presses.
 */

user_inputs fetch_inputs(GLFWwindow* window)
{
    user_inputs inputs;
    glfwGetCursorPos(window, &XPOS(inputs), &YPOS(inputs));
    LMB(inputs) = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    RMB(inputs) = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    K_W(inputs) = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    K_A(inputs) = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    K_S(inputs) = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    K_D(inputs) = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    K_ESC(inputs) = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    return inputs;
}