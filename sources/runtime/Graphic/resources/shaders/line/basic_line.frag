#version 450

layout (location = 0) in vec3 worldPosition;

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 1) in vec4 color;
#endif

layout (location = 0) out vec4 pixel;

layout (push_constant) uniform MaterialInfo {
    layout (offset = 64)
    vec4 color;
} materialInfo;

void main()
{
    pixel = materialInfo.color;
#ifdef COCKTAIL_VERTEX_HAS_COLOR
    pixel *= color;
#endif
}