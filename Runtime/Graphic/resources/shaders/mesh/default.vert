#version 450

layout (location = 0) in vec3 ck_VertexPosition;

layout (binding = 0) uniform CameraInfo {
	mat4 viewProjection;
} cameraInfo;

layout (push_constant) uniform VertexInfo {
	mat4 model;
} vertexInfo;

void main()
{
	vec4 world = vertexInfo.model * vec4(ck_VertexPosition, 1.f);
	gl_Position = cameraInfo.viewProjection * world;
    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}