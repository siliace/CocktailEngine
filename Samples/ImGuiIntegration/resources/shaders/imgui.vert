#version 450

layout (location = 0) in vec2 ck_Position;
layout (location = 1) in vec2 ck_TexCoord;
layout (location = 2) in vec4 ck_Color;

layout (location = 0) out vec2 frag_TexCoord;
layout (location = 1) out vec4 frag_Color;

layout(push_constant) uniform PushConstant { 
	vec2 scale; 
	vec2 translate; 
} geometryInfo;

void main()
{
	frag_TexCoord = ck_TexCoord;
	frag_Color = ck_Color;

	gl_Position = vec4(ck_Position * geometryInfo.scale + geometryInfo.translate, 0.f, 1.f);
}
