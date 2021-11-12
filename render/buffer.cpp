#include "buffer.hpp"

namespace render {

mesh::mesh(shader &shader, GLuint shape) : shader_(shader), shape_(shape),
										   cur_index_(0), bound_(false)
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
}

mesh::~mesh()
{
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glDeleteVertexArrays(1, &vao_);
}

void mesh::bind()
{
	if (bound_)
		return;
	bound_ = true;
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

	enable_vertex_attribs();
}

void mesh::unbind()
{
	if (!bound_)
		return;
	bound_ = false;
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	disable_vertex_attribs();
}

void mesh::draw(shader &shader) const
{
	shader.bind();
	prepare_shader(shader);
	glDrawElements(shape_, count_, GL_UNSIGNED_INT,
				   nullptr); // count and type refer to the index buffer
}

void mesh::update(const game::properties &cur_state)
{
	if (!bound_) bind();
	__update(cur_state);
	draw(shader_);
	unbind();
}

}