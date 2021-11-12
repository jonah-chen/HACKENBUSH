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


// write glDebugMessageCallback to print to console
void APIENTRY
debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			  GLsizei length, const GLchar *message, const void *userParam)
{
	std::cout << "; id: " << id << "; severity: " << severity << "; message: "
			  << message << std::endl;
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

	render::geometry::ground ground(10.0f);
	render::geometry::nodes r_nodes;
	render::geometry::edges r_edges;

	// create and compile shaders
	render::shader shader("/home/hina/Code/HACKENBUSH/render/test_shader.vs",
						  "/home/hina/Code/HACKENBUSH/render/test_shader.fs");
	shader.bind();

	game::edge::container edges;
	game::properties p(glm::vec3(0, 0, 0), edges);


	glm::vec3 bottomleft(-10.0f, 0.0f, -10.0f);
	glm::vec3 topright(10.0f, 10.0f, 10.0f);

	glm::vec3 v1(1.0f, 0.0f, 0.0f);
	glm::vec3 v2(1.0f, 1.0f, 0.0f);

	game::nodes::normal n1(v1);
	game::nodes::normal n2(v2);
	std::vector<game::edge *> buf;
	buf.push_back(game::attach(game::red, &n1, &n2));

	n1.log();
	std::cout << std::endl;

	game::node::container c1;
	n1(c1, bottomleft, topright);

	n1.render(edges);



	// disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    bool playing = true;
	prev_inputs = user_inputs::fetch(window);

	// debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);

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

		prev_inputs = cur_inputs;
		p.pos = camera.get_pos();

		shader.set_uniform("u_mvp", camera.get_view_projection());
		// draw
        if (playing)
        {
            ground.bind();
            ground.update(p);
            shader.set_uniform("u_color", 0.0f, 1.0f, 1.0f, 1.0f);
            ground.draw();
            ground.unbind();
            r_nodes.bind();
            r_nodes.update(p);
            shader.set_uniform("u_color", 1.0f, 0.0f, 0.0f, 1.0f);
            r_nodes.draw();
            r_nodes.unbind();
		// r_edges.bind();
		// r_edges.update(p);
		// r_edges.draw();
		// r_edges.unbind();
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