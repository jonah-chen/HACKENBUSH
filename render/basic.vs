#version 460 core

layout(location = 0) in vec4 pos;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * pos;
}