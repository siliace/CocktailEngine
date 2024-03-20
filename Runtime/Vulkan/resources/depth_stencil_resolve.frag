#version 450

#extension GL_EXT_samplerless_texture_functions : enable

layout (set = 0, binding = 0) uniform texture2DMS inTexture;

#define RESOLVE_MODE_NONE 0
#define RESOLVE_MODE_SAMPLE_ZERO 1
#define RESOLVE_MODE_AVERAGE 2
#define RESOLVE_MODE_MIN 3
#define RESOLVE_MODE_MAX 4

layout (push_constant) uniform ComputeInfo {
    uint sampleCount;
    uint depthResolveMode;
    uint stencilResolveMode;
} computeInfo;

void main()
{
    float depth;
    ivec2 position = ivec2(gl_FragCoord.xy);
    if (computeInfo.depthResolveMode == RESOLVE_MODE_NONE)
    {
        depth = 0;
    }
    if (computeInfo.depthResolveMode == RESOLVE_MODE_SAMPLE_ZERO)
    {
        depth = texelFetch(inTexture, position, 0).r;
    }
    else if (computeInfo.depthResolveMode == RESOLVE_MODE_AVERAGE)
    {
        depth = texelFetch(inTexture, position, 0).r;
        for (int i = 1; i < computeInfo.sampleCount; i++)
            depth = (depth + texelFetch(inTexture, position, i).r) / 2.f;
    }
    if (computeInfo.depthResolveMode == RESOLVE_MODE_MIN)
    {
        depth = 1.f;
        for (int i = 0; i < computeInfo.sampleCount; i++)
            depth = min(depth, texelFetch(inTexture, position, i).r);
    }
    if (computeInfo.depthResolveMode == RESOLVE_MODE_MAX)
    {
        depth = 0.f;
        for (int i = 0; i < computeInfo.sampleCount; i++)
            depth = max(depth, texelFetch(inTexture, position, i).r);
    }
    
    gl_FragDepth = depth;
}