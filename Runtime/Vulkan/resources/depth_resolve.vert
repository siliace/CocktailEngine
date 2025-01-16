#version 450 

void main() 
{
    vec2 outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    gl_Position = vec4(outUV * 2.f + -1.f, 0.f, 1.f);
}