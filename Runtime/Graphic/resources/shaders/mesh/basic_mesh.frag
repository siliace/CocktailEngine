#version 450

layout (location = 0) in vec3 normal;

#ifdef COCKTAIL_VERTEX_HAS_UV
layout (location = 1) in vec2 texCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 2) in vec4 color;
#endif

layout (location = 0) out vec4 pixel;

layout (binding = 0) uniform SceneInfo {
	float ambientFactor;
	vec3 directionalLightColor;
	vec3 directionalLightDirection;
} sceneInfo;

layout (binding = 1) uniform CameraInfo {
	mat4 viewProjection;
} cameraInfo;

#ifdef COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
layout (binding = 2) uniform sampler2D ck_MaterialBaseColor;
#endif

#ifdef COCKTAIL_MATERIAL_HAS_EMISSIVE_TEXTURE
layout (binding = 3) uniform sampler2D ck_MaterialEmissive;
#endif

layout (push_constant) uniform MaterialColors {
	layout (offset = 112)
	vec4 base;
	vec4 specular;
	vec4 emissive;
} materialColors;

void main()
{
	vec4 baseColor = materialColors.base;
	vec3 emissive = materialColors.emissive.rgb;
	
#ifdef COCKTAIL_VERTEX_HAS_COLOR
	baseColor *= color;
#endif

#ifdef COCKTAIL_VERTEX_HAS_UV
	#ifdef COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
		baseColor *= texture(ck_MaterialBaseColor, texCoord);
	#endif

	#ifdef COCKTAIL_MATERIAL_HAS_EMISSIVE_TEXTURE
		emissive *= texture(ck_MaterialEmissive, texCoord);
	#endif
#endif

	vec3 ambientColor = sceneInfo.ambientFactor * baseColor.rgb;

	vec3 lightDirection = -sceneInfo.directionalLightDirection;
	float diffuseFactor = max(dot(normal, lightDirection), 0.f);
	vec3 diffuseColor = sceneInfo.directionalLightColor * diffuseFactor * baseColor.rgb;

	pixel.rgb = ambientColor + diffuseColor;
	pixel.rgb += emissive;
	pixel.a = baseColor.a;
}