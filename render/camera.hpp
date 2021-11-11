#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace render {

class camera 
{
public:
    camera(const glm::vec3 &position, 
           const glm::vec3 &forward = glm::vec3(1.0f, 0.0f, 0.0f), 
           const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 1.0f),
           float fov = glm::radians(70.0f),
           float aspect = 16.0f / 9.0f,
           float near = 0.1f,
           float far = 100.0f);

    inline glm::vec3 get_forward() const { return forward_; }

    glm::mat4 get_view_projection() const;

    void rotate(float pitch, float yaw, float roll);

    void translate(const glm::vec3 &translation);

private:
    glm::mat4 projection_;
    glm::vec3 position_;
    glm::vec3 forward_;
    glm::vec3 up_;
};

}