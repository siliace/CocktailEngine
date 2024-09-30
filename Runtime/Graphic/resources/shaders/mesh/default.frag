#version 450

layout (location = 0) out vec4 pixel;

layout (push_constant) uniform MaterialColors {
	layout (offset = 64)
	vec4 base;
	vec4 specular;
	vec4 emissive;
} materialColors;

void main()
{
	pixel = materialColors.base;
}