#define RELEASE
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "common/profile.hpp"
#include "interaction/input.hpp"
#include "render/shader.hpp"
#include "render/buffer.hpp"
#include "render/camera.hpp"
#include "render/geometry.hpp"
#include "game/game.hpp"

// write glDebugMessageCallback to print to console
void APIENTRY
debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			  GLsizei length, const GLchar *message, const void *userParam)
{
	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		std::cout << "Error " << id << ":" << message << std::endl;
		throw std::runtime_error("OpenGL error");
	}
}


int main(int argc, char *argv[])
{
	// initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return 1;
	}

	// Use opengl 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "HACKENBUSH", nullptr,
										  nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 2;
	}
	// Make the window's context current
	glfwMakeContextCurrent(window);

	// initialize modern opengl
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize OpenGL" << std::endl;
		return 3;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

///////////////////////////////////////////////////////////////////////////////
	hackenbush game;

	if (argc == 1)
	{
		std::cout << "You have not specified a world generation file\n"
					 "A default world would be generated instead.\n";
		game.load_default();
	}
	else
	{
		game.load_world(argv[1]);
	}

	// declare variables here
	user_inputs prev_inputs, cur_inputs;
	game::edge::container edge_container;
	game::properties cur_state(glm::vec3(0.0f, 0.5f, 0.0f), edge_container);
	glm::vec3 bottomleft, topright;
	constexpr float render_distance = 15.0f;
	player player = red_player;

	render::camera camera(glm::vec3(0.0f, 0.5f, 0.0f));

	render::shader basic_shader(

			R"(
#version 330 core
layout(location = 0) in vec4 pos;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * pos;
}
)",

			R"(
#version 330 core

out vec4 color;

uniform vec4 u_color;

void main()
{
    color = u_color;
}

)", 0.0f);

	render::shader edge_shader(

			R"(
#version 330 core

layout(location = 0) in vec4 pos;
layout (location = 1) in vec4 col;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 i_color;

void main()
{
    gl_Position = u_projection * u_view * pos;
    i_color = col;
}
)",

			R"(
#version 330 core

in vec4 i_color;

out vec4 color;

void main()
{
    color = i_color;
}

)", 0.0f);

	// create meshes
	render::geometry::ground ground(basic_shader, render_distance);
	render::geometry::nodes nodes(basic_shader);
	render::geometry::selected_nodes s_nodes(basic_shader);
	render::geometry::edges edges(edge_shader);
	render::geometry::crosshair crosshair(basic_shader, player);

	////////////////////////////////////////////////////////////////////////////


	// disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	bool playing = true;
	prev_inputs = user_inputs::fetch(window);

	// debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT); // clear the screen

		PROFILE_START;

		cur_inputs = user_inputs::fetch(window); // fetch current inputs

		if (K_ESC(cur_inputs) and !K_ESC(prev_inputs))
			playing = !playing;


		if (playing)
		{
			execute_movement(camera, cur_state, cur_inputs, prev_inputs);

			const glm::vec3 right = camera.get_right();
			const glm::vec3 forward = camera.get_forward();

			const float x_min = std::min(std::min(right.x, -right.x),
										 std::min(forward.x + right.x,
												  forward.x - right.x));
			const float x_max = std::max(std::max(right.x, -right.x),
										 std::max(forward.x + right.x,
												  forward.x - right.x));
			const float z_min = std::min(std::min(right.z, -right.z),
										 std::min(forward.z + right.z,
												  forward.z - right.z));
			const float z_max = std::max(std::max(right.z, -right.z),
										 std::max(forward.z + right.z,
												  forward.z - right.z));

			bottomleft = cur_state.pos +
						 glm::vec3(x_min, -1.0f, z_min) * render_distance;
			topright = cur_state.pos +
					   glm::vec3(x_max, 1.0f, z_max) * render_distance;

			cur_state.pos = camera.get_pos();

			cur_state.selected = select(camera, cur_state, cur_inputs, cur_state
					.visible_gamestate);

			if (cur_state.selected and LMB(cur_inputs) and !LMB(prev_inputs))
			{
				game.chop(cur_state.selected, player);
				cur_state.selected = nullptr;
			}
			game.get_visible_edges(cur_state.visible_gamestate, bottomleft,
								   topright);

			camera.set_view_projection(basic_shader);

			ground.update(cur_state);

			camera.set_view_projection(edge_shader);
			edges.update(cur_state);
			nodes.update(cur_state);
			if (cur_state.selected) s_nodes.update(cur_state);
			crosshair.update(cur_state);
		}
		else
		{
			if (RMB(cur_inputs) and !RMB(prev_inputs))
			{
				player = player ? red_player : blue_player;
				crosshair.switch_player();
			}
		}

		prev_inputs = cur_inputs; // update prev_inputs

		PROFILE_LOG;

		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents(); // poll events
	}



	// terminate
	glfwTerminate();
	return 0;
}