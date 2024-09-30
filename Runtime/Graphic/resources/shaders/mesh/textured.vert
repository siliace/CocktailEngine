#version 450

layout (location = 0) in vec3 ck_VertexPosition;
layout (location = 1) in vec2 ck_VertexTexCoord;
layout (location = 2) in vec3 ck_VertexNormal;

layout (location = 0) out vec3 normal;
layout (location = 1) out vec2 texCoord;

layout (binding = 0) uniform CameraInfo {
	mat4 viewProjection;
} cameraInfo;

layout (push_constant) uniform VertexInfo {
	mat4 model;
} vertexInfo;

void main()
{
	normal = (vertexInfo.model * vec4(ck_VertexNormal, 1.f)).xyz;
	texCoord = ck_VertexTexCoord;
	texCoord.y = 1 - texCoord.y;

	vec4 world = vertexInfo.model * vec4(ck_VertexPosition, 1.f);
	gl_Position = cameraInfo.viewProjection * world;
    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}