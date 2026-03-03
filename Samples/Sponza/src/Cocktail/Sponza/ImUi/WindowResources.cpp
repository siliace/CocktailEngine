#include <imgui_internal.h>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

#include <Cocktail/Sponza/ImUi/WindowResources.hpp>

namespace Ck::ImUi
{
    WindowResources::WindowResources(ImGuiContext* context, Renderer::RenderDevice* renderDevice) :
        mNeedInitialization(true)
    {
        ImGuiIO& io = context->IO;

        Byte* pixels;
        int width, height, bytesPerPixel;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytesPerPixel);
        mFontTexturePixels.Append(pixels, width * height * bytesPerPixel);

        Renderer::TextureCreateInfo textureCreateInfo;
        textureCreateInfo.Type = Renderer::TextureType::e2D;
        textureCreateInfo.Size = MakeExtent(static_cast<unsigned int>(width), static_cast<unsigned int>(height), 1u);
        textureCreateInfo.Usage = Renderer::TextureUsageFlagBits::Sampled;
        textureCreateInfo.Format = PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8);
        textureCreateInfo.ArrayLayerCount = 1;
        textureCreateInfo.MipMapsCount = 1;
        mFontTexture = renderDevice->CreateTexture(textureCreateInfo);

        Renderer::TextureViewCreateInfo textureViewCreateInfo;
        textureViewCreateInfo.Texture = mFontTexture;
        textureViewCreateInfo.Type = Renderer::TextureViewType::e2D;
        mFontTextureView = renderDevice->CreateTextureView(textureViewCreateInfo);
    }

    void WindowResources::Initialize(Renderer::RenderContext* renderContext)
    {
        if (!mNeedInitialization)
            return;

        Renderer::CommandList* commandList = renderContext->CreateCommandList({ Renderer::CommandListUsageBits::Transfer });

        Renderer::TextureSubResource fontSubResource = Renderer::TextureSubResource::All(*mFontTexture);
        commandList->Begin(nullptr);
        {
            Renderer::GpuBarrier barriers[] = {
                Renderer::GpuBarrier::Of(mFontTexture.get(), Renderer::ResourceState::Undefined, Renderer::ResourceState::CopyDestination, fontSubResource)
            };
            commandList->Barrier(1, barriers);
        }

        commandList->UploadTextureLevel(mFontTexture.get(), Renderer::ResourceState::CopyDestination, 0, 0, mFontTexturePixels.GetData());

        {
            Renderer::GpuBarrier barriers[] = {
                Renderer::GpuBarrier::Of(mFontTexture.get(), Renderer::ResourceState::CopyDestination, Renderer::ResourceState::GraphicShaderResource, fontSubResource)
            };
            commandList->Barrier(1, barriers);
        }

        commandList->End();

        renderContext->SubmitCommandLists(Renderer::CommandQueueType::Transfer, 1, &commandList, nullptr);
        renderContext->SignalQueue(Renderer::CommandQueueType::Transfer);
        renderContext->WaitQueue(Renderer::CommandQueueType::Graphic, Renderer::CommandQueueType::Transfer);

        mNeedInitialization = false;
    }

    bool WindowResources::NeedInitialization() const
    {
        return mNeedInitialization;
    }

    Renderer::Texture* WindowResources::GetFontTexture() const
    {
        return mFontTexture.get();
    }

    Renderer::TextureView* WindowResources::GetFontTextureView() const
    {
        return mFontTextureView.get();
    }
}
