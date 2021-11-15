/**
 * @file shader.cpp
 * @author Jonah Chen
 * @brief implement shader class specified in shader.hpp
 * @version 1.1
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "shader.hpp"

static void load_shader(GLuint id, const char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file)
		throw std::runtime_error("Failed to open shader file");

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	if (!size)
		throw std::runtime_error("Shader file is empty");
	fseek(file, 0, SEEK_SET);

	char *source = (char *) malloc(size + 1);
	fread(source, 1, size, file);
	source[size] = '\0';
	fclose(file);

	glShaderSource(id, 1, (const char **) &source, nullptr);
	glCompileShader(id);
	free(source);
}

namespace render {

shader *shader::active_shader = nullptr;

shader::shader(const char *vertex_path, const char *fragment_path,
			   const char *geometry_path)
{
	program_ = glCreateProgram();

	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint geometry_shader_id;

	load_shader(vertex_shader_id, vertex_path);
	load_shader(fragment_shader_id, fragment_path);

	if (geometry_path)
	{
		geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		load_shader(geometry_shader_id, geometry_path);
		glAttachShader(program_, geometry_shader_id);
	}

	glAttachShader(program_, vertex_shader_id);
	glAttachShader(program_, fragment_shader_id);

	glLinkProgram(program_);
	glValidateProgram(program_);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	if (geometry_path)
		glDeleteShader(geometry_shader_id);
}

shader::shader(const std::string &vertex_source,
			   const std::string &fragment_source, float dummy, const
			   std::string &geometry_source)
{
	program_ = glCreateProgram();

	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint geometry_shader_id = 0u;

	const char *vertex_source_c = vertex_source.c_str();
	const char *fragment_source_c = fragment_source.c_str();

	glShaderSource(vertex_shader_id, 1, &vertex_source_c, nullptr);
	glShaderSource(fragment_shader_id, 1, &fragment_source_c, nullptr);

	if (!geometry_source.empty())
	{
		geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		const char *geometry_source_c = geometry_source.c_str();
		glShaderSource(geometry_shader_id, 1,
					   (const char **) &geometry_source_c, nullptr);
		glAttachShader(program_, geometry_shader_id);
        glCompileShader(geometry_shader_id);
	}

	glAttachShader(program_, vertex_shader_id);
	glAttachShader(program_, fragment_shader_id);

    glCompileShader(vertex_shader_id);
    glCompileShader(fragment_shader_id);

	glLinkProgram(program_);
	glValidateProgram(program_);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	if (geometry_shader_id)
		glDeleteShader(geometry_shader_id);
}

shader::~shader()
{
	glDeleteProgram(program_);
}

void shader::bind()
{
	if (active_shader != this)
		glUseProgram(program_);
	active_shader = this;
}

void shader::unbind()
{
	if (active_shader == this)
	{
		glUseProgram(0);
		active_shader = nullptr;
	}
}

void shader::set_uniform(const char *name, int value)
{
	bind();
	glUniform1i(locate_uniform(name),
				value);
}

void shader::set_uniform(const char *name, float x, float y, float z, float w)
{
	bind();
	glUniform4f(locate_uniform(name),
				x, y, z, w);
}

void shader::set_uniform(const char *name, const glm::vec4 &data)
{
	bind();
	glUniform4f(locate_uniform(name),
				data.x, data.y, data.z, data.w);
}

void
shader::set_uniform(const char *name, const glm::mat4 &data, bool transpose)
{
	bind();
	glUniformMatrix4fv(locate_uniform(name),
					   1, transpose, glm::value_ptr(data));
}

GLint shader::locate_uniform(const char *name) const
{
	auto uniform_it = uniforms_.find(name);
	if (uniform_it == uniforms_.end())
	{
		GLint location = glGetUniformLocation(program_, name);
		uniforms_[name] = location;
		if (location == -1)
			std::cerr << "Uniform " << name << " not found" << std::endl;
		return location;
	}
	return uniform_it->second;
}

}