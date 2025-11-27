#version 450

#define COCKTAIL_LIGHT_TYPE_DIRECTIONAL 0
#define COCKTAIL_LIGHT_TYPE_POINT 1
#define COCKTAIL_LIGHT_TYPE_SPOT 2

#define COCKTAIL_ALPHA_MODE_OPAQUE 0
#define COCKTAIL_ALPHA_MODE_BLEND 1
#define COCKTAIL_ALPHA_MODE_MASK 2

struct LightInstance 
{
	vec4 color;
	vec3 position;
	float intensity;
	vec3 direction;
	uint type;
	vec2 cutOff;
	float constant;
	float linear;
	float quadratic;
};

layout (location = 0) in vec3 worldPosition;

#ifdef COCKTAIL_VERTEX_HAS_NORMAL
layout (location = 1) in vec3 normal;
#ifdef COCKTAIL_VERTEX_HAS_TANGENT
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
#endif
#endif

#ifdef COCKTAIL_VERTEX_HAS_UV
layout (location = 4) in vec2 texCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 5) in vec4 color;
#endif

layout (location = 0) out vec4 pixel;

layout (set = 0, binding = 0) uniform SceneInfo {
	float ambientFactor;
} sceneInfo;

layout (std430, set = 0, binding = 1) readonly buffer LightInfo {
	LightInstance lightInstances[];
} lightsInfo;

layout (set = 1, binding = 0) uniform CameraInfo {
	mat4 viewProjection;
	vec3 viewDirection;
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

#ifdef COCKTAIL_MATERIAL_HAS_NORMAL_TEXTURE
layout (set = 2, binding = 3) uniform sampler2D ck_MaterialNormal;
#endif

#ifdef COCKTAIL_MATERIAL_HAS_PBR_TEXTURE
layout (set = 2, binding = 4) uniform sampler2D ck_MaterialMetallicRoughness;
#endif

layout (push_constant) uniform MaterialInfo {
	layout (offset = 112)
	vec4 base;
	vec3 emissive;
    float roughness;
    float metallic;
    int alphaMode;
	float alphaCutoff;
} materialInfo;

vec3 DirectionalLight_ComputeLightColor(uint index, in vec3 normal)
{
	LightInstance instance = lightsInfo.lightInstances[index];

	vec3 lightDirection = -instance.direction;
	float diffuseFactor = max(dot(normal, lightDirection), 0.f);

	return instance.color.rgb * diffuseFactor * instance.intensity;
}

vec3 PointLight_ComputeLightColor(uint index, in vec3 normal, in vec3 worldPosition, in vec3 viewDirection)
{
	LightInstance instance = lightsInfo.lightInstances[index];

	vec3 lightToFragment = instance.position - worldPosition;
    vec3 lightDirection = normalize(lightToFragment);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

    // attenuation
    float distance    = length(lightToFragment);
    float attenuation = 1.0 / (instance.constant + instance.linear * distance + instance.quadratic * (distance * distance));    

	return instance.color.rgb * attenuation * diffuseFactor * instance.intensity;
}

vec3 SpotLight_ComputeLightColor(uint index, in vec3 normal, in vec3 worldPosition, in vec3 viewDirection)
{
	LightInstance instance = lightsInfo.lightInstances[index];

	vec3 lightToFragment = instance.position - worldPosition;
    vec3 lightDirection = normalize(lightToFragment);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);

	float theta = dot(lightDirection, normalize(-instance.direction)); 
    float epsilon = (instance.cutOff.x - instance.cutOff.y);
    float coneIntensity = clamp((theta - instance.cutOff.y) / epsilon, 0.0, 1.0);
	float intensity = instance.intensity * coneIntensity;

    // attenuation
    float distance    = length(lightToFragment);
    float attenuation = 1.0 / (instance.constant + instance.linear * distance + instance.quadratic * (distance * distance)); 

	return instance.color.rgb * attenuation * diffuseFactor * intensity;
}

void main()
{
	vec4 baseColor = materialInfo.base;
	vec3 emissive = materialInfo.emissive;
	
#ifdef COCKTAIL_VERTEX_HAS_COLOR
	baseColor *= color;
#endif

#ifdef COCKTAIL_VERTEX_HAS_UV
	#ifdef COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
	if (materialInfo.alphaMode == COCKTAIL_ALPHA_MODE_MASK)
	{
		float alpha = texture(ck_MaterialAlpha, texCoord).r;
		if (alpha < materialInfo.alphaCutoff)
			discard;
	}
	#endif

	#ifdef COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
	baseColor *= texture(ck_MaterialBaseColor, texCoord);
	#endif

	#ifdef COCKTAIL_MATERIAL_HAS_EMISSIVE_TEXTURE
	emissive *= texture(ck_MaterialEmissive, texCoord);
	#endif
#endif

	if (materialInfo.alphaMode == COCKTAIL_ALPHA_MODE_MASK && baseColor.a < materialInfo.alphaCutoff)
		discard;

	vec3 ambientColor = sceneInfo.ambientFactor * baseColor.rgb;
	vec3 diffuseColor = vec3(0);
	
#ifdef COCKTAIL_VERTEX_HAS_NORMAL
    vec3 fragmentNormal = normal;
    #if defined(COCKTAIL_VERTEX_HAS_UV) && defined(COCKTAIL_VERTEX_HAS_TANGENT) && defined(COCKTAIL_MATERIAL_HAS_NORMAL_TEXTURE)
    vec3 n = texture(ck_MaterialNormal, texCoord).xyz;
    n = n * 2.0 - 1.0;

    mat3 tbn = mat3(tangent, biTangent, fragmentNormal);
    fragmentNormal = normalize(tbn * n);
    #endif

	#if defined(COCKTAIL_VERTEX_HAS_UV) && defined(COCKTAIL_MATERIAL_HAS_PBR_TEXTURE)
	vec2 roughnessMetallic = texture(ck_MaterialMetallicRoughness, texCoord).gb;
	float fragmentMetallic = materialInfo.metallic * roughnessMetallic.x;
	float fragmentRoughness = materialInfo.roughness * roughnessMetallic.y;
	#else
	float fragmentMetallic = materialInfo.metallic;
	float fragmentRoughness = materialInfo.roughness;
	#endif

	for (uint i = 0; i < lightsInfo.lightInstances.length(); i++)
	{
		vec3 lightColor;
	    if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_DIRECTIONAL)
		{
			lightColor = DirectionalLight_ComputeLightColor(i, fragmentNormal);
		}
	    else if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_POINT)
		{
			lightColor = PointLight_ComputeLightColor(i, fragmentNormal, worldPosition, cameraInfo.viewDirection);
		}
	    else if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_SPOT)
		{
			lightColor = SpotLight_ComputeLightColor(i, fragmentNormal, worldPosition, cameraInfo.viewDirection);
		}
		diffuseColor += lightColor * baseColor.rgb;
	}
#endif

	pixel.rgb = ambientColor + diffuseColor;
	pixel.rgb += emissive;
	pixel.a = materialInfo.alphaMode != COCKTAIL_ALPHA_MODE_MASK ? baseColor.a : 1.f;
}