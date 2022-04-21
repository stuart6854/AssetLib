#version 460 core

in vec2 frag_TexCoord;
in vec3 frag_Color;

out vec4 out_Color;

uniform vec3 diffuseColor;

uniform sampler2D diffuseTexture;

void main()
{
//    out_Color = vec4(in_Color, 1.0f);
//	  out_Color = vec4(0.8f, 0.3f, 0.2f, 1.0f);
	out_Color = vec4(diffuseColor * texture(diffuseTexture, frag_TexCoord).rgb, 1.0f);
}