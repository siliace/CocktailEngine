#version 450

layout (location = 0) in vec3 ck_VertexPosition;
layout (location = 0) out vec3 worldPosition;

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 1) in vec4 ck_VertexColor;
layout (location = 1) out vec4 color;
#endif

layout (set = 1, binding = 0) uniform CameraInfo {
    mat4 viewProjection;
    vec3 viewDirection;
} cameraInfo;

layout (push_constant) uniform VertexInfo {
    mat4 modelMatrix;
} vertexInfo;

layout (constant_id = 0) const bool USE_VULKAN_COORDINATE_SYSTEM = true;

void main()
{
#ifdef COCKTAIL_VERTEX_HAS_COLOR
    color = ck_VertexColor;
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