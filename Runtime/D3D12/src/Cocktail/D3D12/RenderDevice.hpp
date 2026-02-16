#ifndef COCKTAIL_D3D12_RENDERDEVICE_HPP
#define COCKTAIL_D3D12_RENDERDEVICE_HPP

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

namespace Ck::D3D12
{
    class RenderDevice : public Renderer::RenderDevice
    {
    public:

        RenderDevice();

        std::shared_ptr<Renderer::Buffer> CreateBuffer(const Renderer::BufferCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::BufferView> CreateBufferView(const Renderer::BufferViewCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::Fence> CreateFence(const Renderer::FenceCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::Framebuffer> CreateFramebuffer(const Renderer::FramebufferCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::RenderContext> CreateRenderContext(const Renderer::RenderContextCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::RenderSurface> CreateRenderSurface(const Renderer::RenderSurfaceCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::Sampler> CreateSampler(const Renderer::SamplerCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::Shader> CreateShader(const Renderer::ShaderCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::ShaderProgram> CreateShaderProgram(const Renderer::ShaderProgramCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::Texture> CreateTexture(const Renderer::TextureCreateInfo& createInfo) override;
        std::shared_ptr<Renderer::TextureView> CreateTextureView(const Renderer::TextureViewCreateInfo& createInfo) override;
        bool IsExtensionSupported(Renderer::RenderDeviceExtension extension) const override;
        Renderer::TextureUsageFlags GetTextureFormatSupport(const PixelFormat& format, Renderer::MemoryType memoryType) const override;
        bool IsShadingRateSupported(Extent2D<unsigned int> fragmentSize, Renderer::RasterizationSamples samples) override;
        Signal<LogLevel, Renderer::MessageType, AsciiStringView>& OnDebugMessage() override;
        ID3D12Device* GetHandle() const;

    private:

        Microsoft::WRL::ComPtr<ID3D12Debug> mDebugController;
        Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
        EnumMap<Renderer::CommandQueueType, Microsoft::WRL::ComPtr<ID3D12CommandQueue>> mCommandQueues;
        Signal<LogLevel, Renderer::MessageType, AsciiStringView> mOnDebugMessage;
    };
}

#endif // COCKTAIL_D3D12_RENDERDEVICE_HPP
