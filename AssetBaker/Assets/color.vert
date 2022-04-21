#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Color;

uniform mat4 projView;
uniform mat4 model;

void main()
{
	gl_Position = projView * model * vec4(a_Position, 1.0f);
}