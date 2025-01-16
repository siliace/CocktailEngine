#version 450

#define COCKTAIL_LIGHT_TYPE_DIRECTIONAL 0
#define COCKTAIL_LIGHT_TYPE_POINT 1
#define COCKTAIL_LIGHT_TYPE_SPOT 2

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
#endif

#ifdef COCKTAIL_VERTEX_HAS_UV
layout (location = 2) in vec2 texCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 3) in vec4 color;
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
	
#ifdef COCKTAIL_VERTEX_HAS_NORMAL
	for (uint i = 0; i < lightsInfo.lightInstances.length(); i++)
	{
		vec3 lightColor;
	    if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_DIRECTIONAL)
		{
			lightColor = DirectionalLight_ComputeLightColor(i, normal);
		}
	    else if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_POINT)
		{
			lightColor = PointLight_ComputeLightColor(i, normal, worldPosition, cameraInfo.viewDirection);
		}
	    else if (lightsInfo.lightInstances[i].type == COCKTAIL_LIGHT_TYPE_SPOT)
		{
			lightColor = SpotLight_ComputeLightColor(i, normal, worldPosition, cameraInfo.viewDirection);
		}
		diffuseColor += lightColor * baseColor.rgb;
	}
#endif

	pixel.rgb = ambientColor + diffuseColor;
	pixel.rgb += emissive;
	pixel.a = baseColor.a;
}