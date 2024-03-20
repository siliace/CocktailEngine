#version 450

layout (location = 0) in vec4 frag_Color;

layout (location = 0) out vec4 pixel;

void main() 
{
	pixel = frag_Color;
}