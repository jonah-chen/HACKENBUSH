#version 460 core

layout(location = 0) in vec4 pos;
layout (location = 1) in vec4 col;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 i_color;

void main()
{
    gl_Position = u_projection * u_view * pos;
    i_color = col;
}