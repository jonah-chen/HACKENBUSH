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
#include <unordered_map>

namespace render {

class shader 
{
public:
    shader(const char* vertex_path, const char* fragment_path);

    // there is absolutely no reason to copy shaders.
    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    ~shader();

    void use();

    inline void bind() const { glUseProgram(program_); }
    inline void unbind() const { glUseProgram(0); }

    void set_uniform(const char* name, int value);

    void set_uniform(const char* name, float x, float y, float z, float w);
    void set_uniform(const char* u_name, const glm::vec4 &data);

    void set_uniform(const char* name, const glm::mat4 &data, 
                                                    bool transpose = false);

private:
    GLuint program_;
    mutable std::unordered_map<std::string, GLint> uniforms_;

    GLint locate_uniform(const char* name) const;
};

}