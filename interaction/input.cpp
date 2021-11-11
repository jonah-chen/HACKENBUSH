#include "input.hpp"

user_inputs user_inputs::fetch(GLFWwindow* window)
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

user_inputs user_inputs::operator-(const user_inputs &rhs) const
{
    user_inputs ret;
    for (int i = 0; i < FLOATPOINT_INPUTS; ++i)
        ret.fp[i] = fp[i] - rhs.fp[i];
    for (int i = 0; i < TRUE_FALSE_INPUTS; ++i)
        ret.tf[i] = tf[i] != rhs.tf[i];

    return ret;
}

std::ostream& operator<<(std::ostream &os, const user_inputs &inputs)
{
    os << "xpos:" << XPOS(inputs) << " ypos:" << YPOS(inputs) 
       << " lmb:" << LMB(inputs) << " rmb:" << RMB(inputs) 
       << " W:" << K_W(inputs) << " A:" << K_A(inputs) 
       << " S:" << K_S(inputs) << " D:" << K_D(inputs) 
       << " ESC:" << K_ESC(inputs);
    return os;
}
