#include "input.hpp"

user_inputs user_inputs::fetch(GLFWwindow* window)
{
    user_inputs inputs;
    glfwGetCursorPos(window, &XPOS(inputs), &YPOS(inputs));
    LMB(inputs) = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    RMB(inputs) = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    K_SPACE(inputs) = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    K_LSHIFT(inputs) = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
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


void execute_movement(render::camera& camera, 
                      const game::properties &properties, 
                      const user_inputs &inputs,
                      const user_inputs &last_inputs,
                      float fps)
{
    const float frame_time = 1.0f / fps;
    // the mouse controls the rotation of the camera
    const float mouse_sensitivity = 0.01f;

    const double minus_mouse_delta_x = (XPOS(last_inputs) - XPOS(inputs));
    const double minus_mouse_delta_y = (YPOS(last_inputs) - YPOS(inputs));

    const float yaw = minus_mouse_delta_x * mouse_sensitivity * frame_time;

    const float pitch = minus_mouse_delta_y * mouse_sensitivity * frame_time;


    // no acceleration yet
    const float velocity = 20.0f;
    const float acceleration = 0.0f;

    // the WASD keys control the movement of the camera
    const float forward_speed = (K_W(inputs) - K_S(inputs)) * velocity * frame_time;
    const float right_speed = (K_D(inputs) - K_A(inputs)) * velocity * frame_time;
    const float up_speed = (K_SPACE(inputs) - K_LSHIFT(inputs)) * velocity * frame_time;

    camera.rotate(pitch, yaw);
    camera.translate(forward_speed, up_speed, right_speed);
}
