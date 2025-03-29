#version 450

layout (location = 0) in vec3 ck_VertexPosition;
layout (location = 0) out vec3 worldPosition;

#ifdef COCKTAIL_VERTEX_HAS_NORMAL
layout (location = 1) in vec3 ck_VertexNormal;
layout (location = 1) out vec3 normal;
#endif

#ifdef COCKTAIL_VERTEX_HAS_UV
layout (location = 2) in vec2 ck_VertexTexCoord;
layout (location = 2) out vec2 texCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 3) in vec4 ck_VertexColor;
layout (location = 3) out vec4 color;
#endif

layout (set = 0, binding = 0) uniform SceneInfo {
	float ambientFactor;
} sceneInfo;

layout (set = 1, binding = 0) uniform CameraInfo {
	mat4 viewProjection;
	vec3 viewDirection;
} cameraInfo;

layout (push_constant) uniform VertexInfo {
	mat4 modelMatrix;
	mat3 normalMatrix;
} vertexInfo;

layout (constant_id = 0) const bool USE_VULKAN_COORDINATE_SYSTEM = true;

void main()
{
#ifdef COCKTAIL_VERTEX_HAS_UV
	texCoord = ck_VertexTexCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
    color = ck_VertexColor;
#endif
	
#ifdef COCKTAIL_VERTEX_HAS_NORMAL
	normal = vertexInfo.normalMatrix * ck_VertexNormal;
#endif

	vec4 world = vertexInfo.modelMatrix * vec4(ck_VertexPosition, 1.f);
	worldPosition = world.xyz;

	gl_Position = cameraInfo.viewProjection * world;

	if (USE_VULKAN_COORDINATE_SYSTEM)
	{
		gl_Position.y = -gl_Position.y;
		gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
	}
}