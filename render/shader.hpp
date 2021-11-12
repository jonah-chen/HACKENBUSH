/**
 * @file shader.hpp
 * @author Jonah Chen
 * @brief class for compiling and linking OpenGL shaders. 
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#pragma once
#define GLEW_STATIC

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace render {

class shader
{
private:
	static shader *active_shader;
public:
	shader(const char *vertex_path, const char *fragment_path, const char *geometry_path = nullptr);

	// there is absolutely no reason to copy shaders.
	shader(const shader &) = delete;

	shader &operator=(const shader &) = delete;

	~shader();

	void bind();

	void unbind();

	/**
	 * @brief sets a uniform for the shader.
	 *
	 * @warning the shader will be bound after the uniform is set.
	 *
	 * @param name
	 * @param value
	 */
	void set_uniform(const char *name, int value);

	void set_uniform(const char *name, float x, float y, float z, float w);

	void set_uniform(const char *u_name, const glm::vec4 &data);

	void set_uniform(const char *name, const glm::mat4 &data,
					 bool transpose = false);

private:
	GLuint program_;
	mutable std::unordered_map<std::string, GLint> uniforms_;

	GLint locate_uniform(const char *name) const;
};

}