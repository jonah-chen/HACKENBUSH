/**
 * @file buffer.hpp
 * @author Jonah Chen
 * @brief create, compile, and load vertex and index buffers.
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
#include <game/prereqs.hpp>
#include "shader.hpp"

namespace render {

/**
 * @brief abstract class that should be inherited by all objects or collection 
 * of objects that need to be rendered. 
 * 
 * @note All inherited classes must implement the private method 
 * `void __update(const game::properties&)`.
 * 
 */
class mesh
{
protected:
	/**
	 * @brief Construct a new buffer object. The constructor should:
	 *  - set the shape of the primitive to be rendered.
	 *  - set the `bound_` member to `false`.
	 *  - create vertex and index buffers.
	 *  - initialize the index buffer.
	 *  - throw an exception if any limits are exceeded.
	 *
	 * The constructor should NOT
	 *  - initialize the vertex buffer.
	 *  - call the update method.
	 *  - leave any buffers bound.
	 *
	 * @param shape GLuint describing the shape of the primitive to be rendered.
	 */
	mesh(shader &shader, GLuint shape);

public:
	~mesh();

	mesh(const mesh &) = delete;

	mesh &operator=(const mesh &) = delete;

	/**
	 * @brief Bind the buffers of this object to the current context.
	 *
	 */
	void bind();


	/**
	 * @brief Unbind the buffers of this object from the current context.
	 *
	 */
	void unbind();


	/**
	 * @brief update the state of this object, if necessary to what it should be
	 *  in the current state. This method calls __update(), and keeps the
	 * buffer bound if and only if it was previously bound.
	 *
	 * @param cur_state properties struct describing the current state of the
	 * game.
	 *
	 */
	void update(const game::properties &cur_state);


	/**
	 * @brief Draw the object to the screen.
	 *
	 * @note to self: I have to determine what order to draw the objects.
	 *
	 */
	void draw(shader &shader) const;


	inline bool is_bound() const
	{ return bound_; }

protected:
	GLuint vao_;    // vertex array object
	GLuint vbo_;    // vertex buffer object
	GLuint ebo_;    // element buffer object
	GLuint shape_;  // shape of the primitive to draw
	bool bound_;    // whether the buffer is bound to the current context
	GLsizei count_; // number of elements in the index buffer
	uint32_t cur_index_;    // current index of the buffer
	shader &shader_; // the shader that is made to draw this mesh.

	/**
	 * @brief update the state of this object, if necessary to what it should be
	 * in the current state.
	 *
	 * @pre this object must be bound to the current context.
	 *
	 * @param cur_state properties struct describing the current state of the
	 * game.
	 */
	virtual void __update(const game::properties &cur_state)
	{}

	virtual void prepare_shader(shader &shader) const
	{}

	virtual void enable_vertex_attribs() = 0;

	virtual void disable_vertex_attribs() = 0;

};


}