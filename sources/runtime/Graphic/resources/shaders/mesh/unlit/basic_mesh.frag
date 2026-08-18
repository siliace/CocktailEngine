#version 450

#define COCKTAIL_ALPHA_MODE_OPAQUE 0
#define COCKTAIL_ALPHA_MODE_BLEND 1
#define COCKTAIL_ALPHA_MODE_MASK 2

layout (location = 0) in vec3 worldPosition;

#ifdef COCKTAIL_VERTEX_HAS_UV
layout (location = 1) in vec2 texCoord;
#endif

#ifdef COCKTAIL_VERTEX_HAS_COLOR
layout (location = 2) in vec4 color;
#endif

layout (location = 0) out vec4 pixel;

#ifdef COCKTAIL_MATERIAL_HAS_BASECOLOR_TEXTURE
layout (set = 2, binding = 0) uniform sampler2D ck_MaterialBaseColor;
#endif

#ifdef COCKTAIL_MATERIAL_HAS_EMISSIVE_TEXTURE
layout (set = 2, binding = 1) uniform sampler2D ck_MaterialEmissive;
#endif

#ifdef COCKTAIL_MATERIAL_HAS_ALPHA_TEXTURE
layout (set = 2, binding = 2) uniform sampler2D ck_MaterialAlpha;
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

	pixel.rgb = baseColor.rgb;
	pixel.rgb += emissive;
	pixel.a = materialInfo.alphaMode != COCKTAIL_ALPHA_MODE_MASK ? baseColor.a : 1.f;
}