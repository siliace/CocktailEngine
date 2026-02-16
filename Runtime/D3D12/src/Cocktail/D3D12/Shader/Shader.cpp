#include <Cocktail/D3D12/RenderDevice.hpp>
#include <Cocktail/D3D12/Shader/Shader.hpp>

namespace Ck::D3D12
{
    Shader::Shader(RenderDevice* renderDevice, const Renderer::ShaderCreateInfo& createInfo) :
        mRenderDevice(renderDevice),
        mType(createInfo.Type)
    {
        mBytecode.Resize(createInfo.Code.GetSize());
        Memory::Copy(mBytecode.GetData(), createInfo.Code.GetData(), createInfo.Code.GetSize());
    }

    void Shader::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* Shader::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    Renderer::ShaderType Shader::GetType() const
    {
        return mType;
    }

    Renderer::VertexAttributeLocation* Shader::FindInputAttribute(AsciiStringView name) const
    {
        return nullptr;
    }

    Renderer::VertexAttributeLocation* Shader::FindOutputAttribute(AsciiStringView name) const
    {
        return nullptr;
    }
}
