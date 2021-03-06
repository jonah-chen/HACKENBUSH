/**
 * @file camera.hpp
 * @author Jonah Chen
 * @brief define the camera that is used for rendering 3D objects and control
 * the player's position and orientation in the world. The camera uses a 
 * perspective projection.
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#define GLM_FORCE_RADIANS

#include "common/constants.hpp"
#include "game/prereqs.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace render {

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

	inline glm::vec3 get_up() const
	{ return up_; }

	inline glm::vec3 get_right() const
	{ return right_; }


	void set_view_projection(shader &shader) const;

	void get_viewport(glm::vec3 &bottomleft, glm::vec3 &topright,
					  const game::properties &cur_state,
					  float render_distance_) const;

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