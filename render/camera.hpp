#pragma once

#define GLM_FORCE_RADIANS
#define QUARTERNION_ERROR_TOLERANCE 1e-3f

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace render
{

    class camera
    {
    public:
        camera(const glm::vec3 &pos,
               const glm::vec3 &forward = glm::vec3(1.0f, 0.0f, 0.0f),
               const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
               float fov = 1.4f,
               float aspect = 16.0f / 9.0f,
               float near = 0.1f,
               float far = 100.0f,
               float ground_level = 0.5f);

        inline glm::vec3 get_forward() const
        { return forward_; }

        inline glm::vec3 get_pos() const
        { return pos_; }

        glm::mat4 get_view_projection() const;

        /**
         * @brief
         *
         * @param pitch float of pitch angle in radians
         * @param yaw float of yaw angle in radians
         * @note roll is not supported because it is not needed in this type of
         * game.
         */
        void rotate(float pitch, float yaw);

        void translate(float forward, float up, float right);

    private:
        glm::mat4 projection_;
        glm::vec3 pos_;
        glm::vec3 forward_;
        glm::vec3 up_;
        glm::vec3 right_;
        float ground_level_;
    };

}