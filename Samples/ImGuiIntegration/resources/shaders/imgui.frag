#version 450

layout (location = 0) in vec2 frag_TexCoord;
layout (location = 1) in vec4 frag_Color;

layout (location = 0) out vec4 pixel;

layout (set = 0, binding = 0) uniform sampler2D fontTexture;

void main() 
{
	pixel = frag_Color * texture(fontTexture, frag_TexCoord);
}