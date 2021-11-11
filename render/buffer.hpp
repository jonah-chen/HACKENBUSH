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

namespace render {

/**
 * @brief abstract class that should be inherited by all objects or collection 
 * of objects that need to be rendered.
 * 
 */
class buffer
{
public:
    ~buffer();

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
     *  in the current state.
     * 
     */
    virtual void update(const game::properties &cur_state) {}


    /**
     * @brief Draw the object to the screen. 
     * 
     * @note to self: I have to determine what order to draw the objects.
     * 
     */
    void draw() const;

    
    inline bool is_bound() const { return bound_; }

protected:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    GLuint shape_;
    bool bound_;
    GLsizei count_;
    uint32_t cur_index_;
    
    buffer(GLuint shape);
};


}