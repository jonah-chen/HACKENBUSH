/**
 * @file camera.cpp
 * @author Jonah Chen
 * @brief implement the camera functionality for rendering specified in `camera.hpp`
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "camera.hpp"

namespace render {

camera::camera(const glm::vec3 &pos,
			   const glm::vec3 &forward,
			   const glm::vec3 &up,
			   float fov, float aspect, float znear, float zfar,
			   float ground_level)
		: pos_(pos), ground_level_(ground_level)
{
	projection_ = glm::perspective(fov, aspect, znear, zfar);
	forward_ = glm::normalize(forward);
	up_ = glm::normalize(up);
	right_ = glm::cross(forward_, up_);

	assert(!glm::dot(forward_, up_));
}

void camera::rotate(float pitch, float yaw)
{
	glm::mat4 rotation = glm::mat4(1.0f);

	rotation = glm::rotate(rotation, pitch,
						   glm::vec3(right_.x, 0.0f, right_.z));
	rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	// quaternion rotations can cause floating point errors.
	// correct using the small angle approximation.
	if ((right_.y > QUARTERNION_ERROR_TOLERANCE
		 or right_.y < -QUARTERNION_ERROR_TOLERANCE) and up_.y > 0.5f)
		rotation = glm::rotate(rotation, right_.y,
							   glm::vec3(forward_.x, 0.0f, forward_.z));

	forward_ = glm::normalize(glm::vec3(rotation * glm::vec4(forward_, 1.0f)));
	up_ = glm::normalize(glm::vec3(rotation * glm::vec4(up_, 1.0f)));
	right_ = glm::normalize(glm::cross(forward_, up_));
}

void camera::translate(float forward, float up, float right)
{
	pos_.x += forward * forward_.x + right * right_.x;
	pos_.y += up;
	pos_.z += forward * forward_.z + right * right_.z;

	if (pos_.y < ground_level_)
		pos_.y = ground_level_;
}

void camera::set_view_projection(shader &shader) const
{
	shader.set_uniform("u_view", glm::lookAt(pos_, pos_ + forward_, up_));
	shader.set_uniform("u_projection", projection_);
}

void camera::get_viewport(glm::vec3 &bottomleft, glm::vec3 &topright, const
game::properties &cur_state, float render_distance) const
{
	const float x_min = std::min(std::min(right_.x, -right_.x),
								 std::min(forward_.x + right_.x,
										  forward_.x - right_.x));
	const float x_max = std::max(std::max(right_.x, -right_.x),
								 std::max(forward_.x + right_.x,
										  forward_.x - right_.x));
	const float z_min = std::min(std::min(right_.z, -right_.z),
								 std::min(forward_.z + right_.z,
										  forward_.z - right_.z));
	const float z_max = std::max(std::max(right_.z, -right_.z),
								 std::max(forward_.z + right_.z,
										  forward_.z - right_.z));

	bottomleft = cur_state.pos +
				 glm::vec3(x_min, -1.0f, z_min) * render_distance;
	topright = cur_state.pos +
			   glm::vec3(x_max, 1.0f, z_max) * render_distance;
}

}
