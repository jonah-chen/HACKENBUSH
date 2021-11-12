#define RELEASE
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "common/profile.hpp"
#include "interaction/input.hpp"
#include "render/shader.hpp"


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
	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	// create vertex array and index buffer objects to draw one triangle

	float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
			0, 1, 2
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// create and compile shaders
	render::shader shader("/home/hina/Code/HACKENBUSH/render/test_shader.vs",
						  "/home/hina/Code/HACKENBUSH/render/test_shader.fs");
	shader.bind();
	shader.set_uniform("u_color", 0.0f, 1.0f, 1.0f, 1.0f);


	// disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	// set the initial mouse position
	glfwGetCursorPos(window, &XPOS(prev_inputs), &YPOS(prev_inputs));

	prev_inputs = user_inputs::fetch(window);

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		PROFILE_START;
		cur_inputs = user_inputs::fetch(window);

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		// std::cout << cur_inputs - prev_inputs << std::endl;
		prev_inputs = cur_inputs;

		// draw
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);


		PROFILE_LOG;

		// swap buffers
		glfwSwapBuffers(window);// this is what takes up the rest of the time
		// to maintain 60fps.
		// poll events
		glfwPollEvents();
	}

	// terminate
	glfwTerminate();
	return 0;
}