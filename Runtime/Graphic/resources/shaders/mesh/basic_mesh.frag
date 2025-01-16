#version 450

#define COCKTAIL_LIGHT_TYPE_DIRECTIONAL 0
#define COCKTAIL_LIGHT_TYPE_POINT 1
#define COCKTAIL_LIGHT_TYPE_SPOT 2

struct LightInstance 
{
	uint type;
	vec4 color;
	vec4 position;
	vec3 direction;
	vec2 cutOff;
	float constant;
	float linear;
	float quadratic;
};

layout (location = 0) in vec3 normal;

#ifdef COCKTAIL_VERTEX_HAS_UV
layout (location = 1) in vec2 texCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 2) in vec4 color;
#endif

layout (location = 0) out vec4 pixel;

layout (set = 0, binding = 0) uniform SceneInfo {
	float ambientFactor;
} sceneInfo;

layout (std140, set = 0, binding = 1) readonly buffer LightInfo {
	uint lightInstanceCount;
	LightInstance lightInstances[];
} lightsInfo;

layout (set = 1, binding = 0) uniform CameraInfo {
	mat4 viewProjection;
} cameraInfo;

#ifdef COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
layout (set = 2, binding = 0) uniform sampler2D ck_MaterialBaseColor;
#endif

#ifdef COCKTAIL_MATERIAL_HAS_EMISSIVE_TEXTURE
layout (set = 2, binding = 1) uniform sampler2D ck_MaterialEmissive;
#endif

#ifdef COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
layout (set = 2, binding = 2) uniform sampler2D ck_MaterialAlpha;
#endif

layout (push_constant) uniform MaterialColors {
	layout (offset = 112)
	vec4 base;
	vec3 specular;
	float padding_1;
	vec3 emissive;
	float padding_2;
} materialColors;

vec3 DirectionalLight_ComputeLightColor(uint index, in vec3 normal)
{
	LightInstance instance = lightsInfo.lightInstances[index];

	vec3 lightDirection = -instance.direction;
	float diffuseFactor = max(dot(normal, lightDirection), 0.f);
	return instance.color.rgb * diffuseFactor;
}

vec3 PointLight_ComputeLightColor(uint index, in vec3 normal, in vec3 worldPosition, in vec3 viewDirection)
{
	return vec3(0.f);
}

void main()
{
	vec4 baseColor = materialColors.base;
	vec3 emissive = materialColors.emissive;
	
#ifdef COCKTAIL_VERTEX_HAS_COLOR
	baseColor *= color;
#endif

#ifdef COCKTAIL_VERTEX_HAS_UV
	#ifdef COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
	float alpha = texture(ck_MaterialAlpha, texCoord).r;
	if (alpha == 0)
		discard;
	#endif

	#ifdef COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
	baseColor *= texture(ck_MaterialBaseColor, texCoord);
	#endif

	#ifdef COCKTAIL_MATERIAL_HAS_EMISSIVE_TEXTURE
	emissive *= texture(ck_MaterialEmissive, texCoord);
	#endif
#endif

	vec3 ambientColor = sceneInfo.ambientFactor * baseColor.rgb;
	vec3 diffuseColor = vec3(0);
	
	for (uint i = 0; i < lightsInfo.lightInstanceCount; i++)
	{
		vec3 lightColor;
	    if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_DIRECTIONAL)
		{
			lightColor = DirectionalLight_ComputeLightColor(i, normal);
		}
	    else if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_POINT)
		{
			lightColor = PointLight_ComputeLightColor(i, normal, vec3(0.f), vec3(0.f));
		}
		diffuseColor += lightColor * baseColor.rgb;
	}

	pixel.rgb = ambientColor + diffuseColor;
	pixel.rgb += emissive;
	pixel.a = baseColor.a;
}