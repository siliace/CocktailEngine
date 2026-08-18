#version 450 core

layout(location = 0) in vec2 vPos;
layout(location = 1) in vec2 vUv;
layout(location = 2) in vec4 vColor;

layout (location = 0) out vec4 color;
layout (location = 1) out vec2 uv;

layout(push_constant) uniform VertexInfo {
    vec2 scale;
    vec2 translate;
} vertexInfo;

void main()
{
    color = vColor;
    uv = vUv;
    gl_Position = vec4(vPos * vertexInfo.scale + vertexInfo.translate, 0, 1);
}
