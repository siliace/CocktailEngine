#version 450

layout (location = 0) in vec3 ck_Position;
layout (location = 1) in vec4 ck_Color;

layout (location = 0) out vec4 frag_Color;

void main()
{
	frag_Color = ck_Color;

	gl_Position = vec4(ck_Position, 1.f);
}
