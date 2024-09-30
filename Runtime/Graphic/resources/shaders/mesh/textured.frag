#version 450

layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec4 pixel;

layout (push_constant) uniform MaterialColors {
	layout (offset = 64)
	vec4 base;
	vec4 specular;
	vec4 emissive;
} materialColors;

layout (binding = 1) uniform sampler2D ck_MaterialBaseColor;

void main()
{
	vec4 diffuse = materialColors.base;
	diffuse *= texture(ck_MaterialBaseColor, texCoord);

	pixel = diffuse;
}