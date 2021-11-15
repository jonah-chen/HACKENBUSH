/**
 * @file main.cxx
 * @author Jonah Chen
 * @brief The client for the hackenbush game. Contains the main loop.
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#define RELEASE
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "common/profile.hpp"
#include "interaction/input.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/geometry.hpp"
#include "game/game.hpp"

/**
 * @brief Initialize the current OpenGL context and GLFW window for the game.
 * 
 * @param window reference to the GLFW window pointer, which will store the 
 * created window.
 * @return 0 on success.
 * @return nonzero number on failure, and an error message is printed to stderr.
 */
static int init(GLFWwindow **window);

/**
 * @brief parse the arguments to initialize the program.
 * 
 * @param argc the number of arguments.
 * @param argv the array of arguments c-strings
 * @param player reference to the player value, which will store the player who 
 * goes first.
 * @return the location of `argv` that contains the file path of the world file. 
 */
static int parse_args(int argc, char **argv, player &player);

/**
 * @brief switch the player to the next player, this is usually done in single 
 * player mode when the current player is done with chopping a branch or passes 
 * their turn.
 * 
 * @param player reference to the player value, which stores the current player. 
 * @param crosshair reference to the crosshair that is currently rendered.
 */
static inline void switch_player(player &player,
								 render::geometry::crosshair &crosshair);

/**
 * @brief callback that will catch OpenGL errors, and print them to stderr.
 */
static void APIENTRY
debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			  GLsizei length, const GLchar *message, const void *userParam);

int main(int argc, char *argv[])
{
	hackenbush game;
	user_inputs prev_inputs, cur_inputs;
	game::edge::container edge_container;
	glm::vec3 bottomleft, topright;

	bool GAME_SINGLE_PLAYER = true; // automatically switches between red and
	// blue player.
	player player = red_player;     // red goes first if not specified.
	game::properties cur_state(glm::vec3(0.0f, 0.5f, 0.0f), edge_container);
	bool playing = true;
	constexpr float render_distance = 15.0f;

	// parse arguments
	int parse_code = parse_args(argc, argv, player);
	if (parse_code)
		game.load_world(argv[parse_code]);
	else
		game.load_default();

	GLFWwindow *window;
	if (init(&window))
		throw std::runtime_error("Error!");


	// create the camera
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


	// rendering loop
	prev_inputs = user_inputs::fetch(window);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT); // clear the screen

		PROFILE_START;

		cur_inputs = user_inputs::fetch(window); // fetch current inputs

		if (DOWN(K_ESC, cur_inputs, prev_inputs))
		{
			if (playing)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			playing = !playing;
		}

		if (playing)
		{
			execute_movement(camera, cur_state, cur_inputs, prev_inputs);

			camera.get_viewport(bottomleft, topright, cur_state,
								render_distance);

			cur_state.pos = camera.get_pos();

			cur_state.selected_branch = select(camera, cur_state, cur_inputs,
											   cur_state.visible_gamestate);

			if (cur_state.selected_branch and
				DOWN(LMB, cur_inputs, prev_inputs) and
				game.chop(cur_state.selected_branch, player))
			{
				cur_state.selected_branch = nullptr;
				if (GAME_SINGLE_PLAYER)
					switch_player(player, crosshair);
			}

			if (DOWN(K_P, cur_inputs, prev_inputs))
				switch_player(player, crosshair);

			game.get_visible_edges(cur_state.visible_gamestate, bottomleft,
								   topright);

			camera.set_view_projection(basic_shader);
			ground.update(cur_state);
			camera.set_view_projection(edge_shader);
			edges.update(cur_state);
			nodes.update(cur_state);
			if (cur_state.selected_branch)
				s_nodes.update(cur_state);
			crosshair.update(cur_state);
		}
		else
		{
			if (DOWN(RMB, cur_inputs, prev_inputs))
				game.command_terminal();
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


static void APIENTRY
debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			  GLsizei length, const GLchar *message, const void *userParam)
{
	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		std::cerr << "Error " << id << ":" << message << std::endl;
		throw std::runtime_error("OpenGL error");
	}
}

static int init(GLFWwindow **window)
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
	*window = glfwCreateWindow(1920, 1080, "HACKENBUSH", nullptr,
							   nullptr);
	if (!*window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 2;
	}
	// Make the window's context current
	glfwMakeContextCurrent(*window);

	// initialize modern opengl
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize OpenGL" << std::endl;
		return 3;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// disable cursor
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

static int parse_args(int argc, char **argv, player &player)
{
	if (argc == 1)
	{
		std::cout << "You have not specified a world generation file\n"
					 "A default world would be generated instead.\n";
		return 0;
	}
	else
	{
		if (strcmp(argv[1], "--help") == 0 or strcmp(argv[1], "-h") == 0)
		{
			std::cout << "Usage: hackenbush [world_file] [first_player: "
						 "-R/-B]\n"
                         "- If the world specified is 0, an empty world will be"
                         " generated\n"
						 "- If no world file is specified, a default world will "
						 "be generated.\n";
			exit(0);
		}
		else if (strcmp(argv[1], "-R") == 0)
			player = red_player;
		else if (strcmp(argv[1], "-B") == 0)
			player = blue_player;
		else return 1;

		if (argc == 3)
		{
			if (strcmp(argv[2], "-R") == 0)
				player = red_player;
			else if (strcmp(argv[2], "-B") == 0)
				player = blue_player;
			else return 2;
		}

		return 0;
	}
}

static inline void switch_player(player &player, render::geometry::crosshair
&crosshair)
{
	if (player == red_player)
		player = blue_player;
	else if (player == blue_player)
		player = red_player;
	crosshair.switch_player();
}
