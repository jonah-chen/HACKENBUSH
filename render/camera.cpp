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

glm::mat4 camera::get_view_projection() const
{
	return projection_ * glm::lookAt(pos_, pos_ + forward_, up_);
}

void camera::rotate(float pitch, float yaw)
{
	glm::mat4 rotation = glm::mat4(1.0f);

	rotation = glm::rotate(rotation, pitch,
						   glm::vec3(right_.x, 0.0f, right_.z));
	rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	forward_ = glm::vec3(rotation * glm::vec4(forward_, 1.0f));
	up_ = glm::vec3(rotation * glm::vec4(up_, 1.0f));
	right_ = glm::cross(forward_, up_);
}

void camera::translate(float forward, float up, float right)
{
	pos_.x += forward * forward_.x + right * right_.x;
	pos_.y += up;
	pos_.z += forward * forward_.z + right * right_.z;

	if (pos_.y < ground_level_)
		pos_.y = ground_level_;
}

}
