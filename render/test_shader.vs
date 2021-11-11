#version 460 core

layout(location = 0) in vec4 pos;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * pos;
}