#include "shader.hpp"

namespace render {

shader::shader(const char *vertex_shader, const char *fragment_shader)
{
	program_ = glCreateProgram();
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream vertex_shader_file(vertex_shader);
	std::ifstream fragment_shader_file(fragment_shader);

	std::string vertex_shader_string(
			(std::istreambuf_iterator<char>(vertex_shader_file)),
			std::istreambuf_iterator<char>());
	std::string fragment_shader_string(
			(std::istreambuf_iterator<char>(fragment_shader_file)),
			std::istreambuf_iterator<char>());

	if (vertex_shader_string.empty() or fragment_shader_string.empty())
		throw std::runtime_error("Could not load shader");

	const char *vertex_shader_cstr = vertex_shader_string.c_str();
	const char *fragment_shader_cstr = fragment_shader_string.c_str();

	glShaderSource(vertex_shader_id, 1, &vertex_shader_cstr, nullptr);
	glShaderSource(fragment_shader_id, 1, &fragment_shader_cstr, nullptr);

	glCompileShader(vertex_shader_id);
	glCompileShader(fragment_shader_id);

	glAttachShader(program_, vertex_shader_id);
	glAttachShader(program_, fragment_shader_id);
	glLinkProgram(program_);
	glValidateProgram(program_);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
}

shader::~shader()
{
	glDeleteProgram(program_);
}

void shader::set_uniform(const char *name, int value)
{
	glUniform1i(locate_uniform(name),
				value);
}

void shader::set_uniform(const char *name, float x, float y, float z, float w)
{
	glUniform4f(locate_uniform(name),
				x, y, z, w);
}

void shader::set_uniform(const char *name, const glm::vec4 &data)
{
	glUniform4f(locate_uniform(name),
				data.x, data.y, data.z, data.w);
}

void
shader::set_uniform(const char *name, const glm::mat4 &data, bool transpose)
{
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