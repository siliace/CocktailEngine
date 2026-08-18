#version 450 core

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec4 pixel;

layout (set = 0, binding = 0) uniform sampler2D inTexture;

void main()
{
    pixel = color * texture(inTexture, uv);
}
