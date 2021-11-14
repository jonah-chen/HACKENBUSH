#include "input.hpp"

/**
 * @brief calculate the minimum distance between a ray and a line segment. The 
 * ray is defined by the forward and position vectors, and the line segment is 
 * described by the two endpoints p1 and p2.
 * 
 * @param forward a vec3 describing the normalized forward vector of the camera.
 * @param position a vec3 describing the position of the camera.
 * @param p1 a vec3 describing the position of one endpoint of a branch.
 * @param p2 a vec3 describing the position of the other endpoint of a branch.
 * @return a float describing the minimum distance between the ray of the 
 * forward vector and the branch.
 * @return infinity if the branch is behind the player.
 */
static float calc_min_distance(const glm::vec3 &forward, const glm::vec3 &pos,
							   const glm::vec3 &p1, const glm::vec3 &p2)
{
	// check if the line segment is in behind of the camera
	const glm::vec3 to_p1 = p1 - pos;
	const glm::vec3 to_p2 = p2 - pos;

	if (glm::dot(forward, to_p2) <= 0 and glm::dot(forward, to_p1) <= 0)
		return std::numeric_limits<float>::infinity();

	const float min_dist_to_p1 = glm::length(glm::cross(forward, to_p1));
	const float min_dist_to_p2 = glm::length(glm::cross(forward, to_p2));
	const float min_dist_to_endpoint = glm::min(min_dist_to_p1, min_dist_to_p2);
	const float dist_to_midpoint = glm::length(glm::cross(forward,
														  (to_p2 + to_p1) *
														  0.5f));

	const glm::vec3 normal = glm::cross(forward, p2 - p1);
	const float line_min_dist = glm::abs(
			glm::dot(pos - p1, normal) / glm::length
					(normal));

	return min_dist_to_endpoint < line_min_dist or
		   min_dist_to_endpoint < dist_to_midpoint
		   ? min_dist_to_endpoint
		   : line_min_dist;
}

user_inputs user_inputs::fetch(GLFWwindow *window)
{
	user_inputs inputs;
	glfwGetCursorPos(window, &XPOS(inputs), &YPOS(inputs));
	LMB(inputs) =
			glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	RMB(inputs) =
			glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	K_SPACE(inputs) = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	K_LSHIFT(inputs) = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	K_W(inputs) = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	K_A(inputs) = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	K_S(inputs) = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	K_D(inputs) = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	K_ESC(inputs) = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	K_P(inputs) = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
    K_LCTRL(inputs) = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
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


std::ostream &operator<<(std::ostream &os, const user_inputs &inputs)
{
	os << "xpos:" << XPOS(inputs) << " ypos:" << YPOS(inputs)
	   << " lmb:" << LMB(inputs) << " rmb:" << RMB(inputs)
	   << " W:" << K_W(inputs) << " A:" << K_A(inputs)
	   << " S:" << K_S(inputs) << " D:" << K_D(inputs)
	   << " SPACE:" << K_SPACE(inputs) << " LSHIFT:" << K_LSHIFT(inputs)
	   << " ESC:" << K_ESC(inputs) << " P:" << K_P(inputs) 
       << " LCTRL:" << K_LCTRL(inputs);
	return os;
}


void
execute_movement(render::camera &camera, const game::properties &properties,
				 const user_inputs &inputs,
				 const user_inputs &last_inputs,
				 float fps)
{
	const float frame_time = 1.0f / fps;
	// the mouse controls the rotation of the camera
	const float mouse_sensitivity = 0.027f;

	const double minus_mouse_delta_x = (XPOS(last_inputs) - XPOS(inputs));
	const double minus_mouse_delta_y = (YPOS(last_inputs) - YPOS(inputs));

	const float yaw = minus_mouse_delta_x * mouse_sensitivity * frame_time;

	const float pitch = minus_mouse_delta_y * mouse_sensitivity * frame_time;


	// no acceleration yet
	float velocity = K_LCTRL(inputs) ? 10.0f : 2.0f;
	const float acceleration = 0.0f;

	// the WASD keys control the movement of the camera
	const float forward_speed =
			(K_W(inputs) - K_S(inputs)) * velocity * frame_time;
	const float right_speed =
			(K_D(inputs) - K_A(inputs)) * velocity * frame_time;
	const float up_speed =
			(K_SPACE(inputs) - K_LSHIFT(inputs)) * velocity * frame_time;

	camera.rotate(pitch, yaw);
	camera.translate(forward_speed, up_speed, right_speed);
}

game::edge *select(const render::camera &camera, const game::properties
&properties, const user_inputs &inputs, const game::edge::container &candidates)
{
	float min_dist = MIN_WACK_DISTANCE;
	game::edge *selected = nullptr;
	for (game::edge *candidate: candidates)
	{
		const glm::vec3 p1 = candidate->p1->get_pos();
		const glm::vec3 p2 = candidate->p2->get_pos();
		const float dist = calc_min_distance(camera.get_forward(),
											 camera.get_pos(), p1, p2);
		if (dist < min_dist)
		{
			min_dist = dist;
			selected = candidate;
		}
	}
	return selected;
}
