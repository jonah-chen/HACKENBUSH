//#define RELEASE
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
#include "game/generators.hpp"


// write glDebugMessageCallback to print to console
void APIENTRY
debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			  GLsizei length, const GLchar *message, const void *userParam)
{
	std::cout << "; id: " << id << "; severity: " << severity << "; message: "
			  << message << std::endl;
	if (severity == GL_DEBUG_SEVERITY_HIGH)
        exit(1);
}


int main(int argc, char **argv)
{
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
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "Hello World", NULL,
										  NULL);
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


	user_inputs prev_inputs, cur_inputs;

	render::camera camera(glm::vec3(0.0f, 0.5f, 0.0f));

	// create meshes
	render::geometry::ground ground(10.0f);
	render::geometry::nodes r_nodes;
	render::geometry::edges r_edges;
	render::geometry::crosshair r_crosshair;

	// create and compile shaders
	render::shader shader("/home/hina/Code/HACKENBUSH/render/basic.vs",
						  "/home/hina/Code/HACKENBUSH/render/basic.fs");
	render::shader shader2("/home/hina/Code/HACKENBUSH/render/edge.vs",
						   "/home/hina/Code/HACKENBUSH/render/edge.fs");

	game::edge::container edges;
	game::properties p(glm::vec3(0, 0, 0), edges);


	glm::vec3 bottomleft(-10.0f, 0.0f, -10.0f);
	glm::vec3 topright(10.0f, 10.0f, 10.0f);

	glm::vec3 v1(8.0f, 0.0f, 0.0f);
	glm::vec3 v2(8.0f, 1.0f, 0.0f);
	glm::vec3 v3(8.0f, 2.0f, 0.0f);
	glm::vec3 v4(8.0f, 2.0f, 1.0f);

	game::nodes::normal n1(v1);
	game::nodes::normal n2(v2);
	game::nodes::normal n3(v3);
	game::nodes::stack_root n4(v4, glm::vec3(0.0f, 3.0f, 0.0f), ALL_GREEN,
							GEOMETRIC, nullptr);

	std::vector<game::edge *> buf;
	buf.push_back(game::attach(game::green, &n1, &n2));
	buf.push_back(game::attach(game::blue, &n2, &n3));
	buf.push_back(game::attach(game::red, &n2, &n4));
	n2.log();

	std::cout << std::endl;

	game::node::container c1;
	n1(c1, bottomleft, topright);
	for (auto *n : c1)
		n->render(edges);



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
		PROFILE_START;
		cur_inputs = user_inputs::fetch(window);
        if (K_ESC(cur_inputs) and !K_ESC(prev_inputs))
            playing = !playing;
		// render
		glClear(GL_COLOR_BUFFER_BIT);

		execute_movement(camera, p, cur_inputs, prev_inputs);

		//std::cout << select(camera, p, cur_inputs, edges) << std::endl;


		prev_inputs = cur_inputs;
		p.pos = camera.get_pos();

		// draw
        if (playing)
		{
			camera.set_view_projection(shader);

			ground.bind();
			ground.update(p);
			ground.draw(shader);
			ground.unbind();

			camera.set_view_projection(shader2);

			r_edges.bind();
			r_edges.update(p);
			r_edges.draw(shader2);
			r_edges.unbind();

			r_nodes.bind();
			r_nodes.update(p);
			r_nodes.draw(shader);
			r_nodes.unbind();

			r_crosshair.bind();
			r_crosshair.update(p);
			r_crosshair.draw(shader);
			r_crosshair.unbind();

		}
		PROFILE_LOG;

		// swap buffers
		glfwSwapBuffers(window);// this is what takes up the rest of the time
		// to maintain 60fps.
		// poll events
		glfwPollEvents();
	}

	for (auto *e: buf)
		delete e;


	// terminate
	glfwTerminate();
	return 0;
}