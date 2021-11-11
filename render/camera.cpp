#include "camera.hpp"

namespace render {

camera::camera(const glm::vec3 &position, 
               const glm::vec3 &forward,
               const glm::vec3 &up,
               float fov, float aspect, float znear, float zfar)
    : position_(position), forward_(forward), up_(up)
{
    projection_ = glm::perspective(fov, aspect, znear, zfar);
}

glm::mat4 camera::get_view_projection() const
{
    return projection_ * glm::lookAt(position_, position_ + forward_, up_);
}

void camera::rotate(float pitch, float yaw, float roll)
{
    glm::mat4 rotation = glm::mat4(1.0f);

    rotation = glm::rotate(rotation, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, roll, glm::vec3(0.0f, 0.0f, 1.0f));

    forward_ = glm::vec3(rotation * glm::vec4(forward_, 1.0f));
    up_ = glm::vec3(rotation * glm::vec4(up_, 1.0f));
}

void camera::translate(const glm::vec3 &translation)
{
    position_ += translation;
}

}
