#include "buffer.hpp"

namespace render {

buffer::buffer(GLuint shape) : shape_(shape), cur_index_(0)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}

buffer::~buffer()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
    glDeleteVertexArrays(1, &vao_);
}

void buffer::bind()
{
    if (bound_)
        return;
    bound_ = true;
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}

void buffer::unbind()
{
    if (!bound_)
        return;
    bound_ = false;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void buffer::draw() const
{
    glDrawElements(shape_, count_, GL_UNSIGNED_INT, 0); // count and type refer to the index buffer
}


}