#ifndef COCKTAILENGINE_SHADER_HPP
#define COCKTAILENGINE_SHADER_HPP

#include <d3d12.h>
#include <d3dcommon.h>

#include <Cocktail/Renderer/Shader/Shader.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class Shader : public Renderer::Shader
    {
    public:

        Shader(RenderDevice* renderDevice, const Renderer::ShaderCreateInfo& createInfo);
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        Renderer::ShaderType GetType() const override;
        Renderer::VertexAttributeLocation* FindInputAttribute(AsciiStringView name) const override;
        Renderer::VertexAttributeLocation* FindOutputAttribute(AsciiStringView name) const override;

    private:

        Renderer::RenderDevice* mRenderDevice;
        Renderer::ShaderType mType;
        ByteArray mBytecode;
    };
}

#endif // COCKTAILENGINE_SHADER_HPP
