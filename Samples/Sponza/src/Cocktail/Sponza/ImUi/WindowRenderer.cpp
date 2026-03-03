#include <imgui_internal.h>

#include <Cocktail/Core/Math/Vector/Vector2.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/StorageUtils.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Buffer/BufferAllocator.hpp>
#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Command/RenderPassBeginInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>

#include <Cocktail/Sponza/ImUi/WindowRenderer.hpp>

namespace Ck::ImUi
{
    WindowRenderer::WindowRenderer(Renderer::RenderDevice* renderDevice)
    {
        auto loadShader = [&](Renderer::ShaderType shaderType, const URI& uri) {
            ByteArray fileContent = StorageUtils::ReadFile(uri);

            Renderer::ShaderCreateInfo shaderCreateInfo;
            shaderCreateInfo.Type = shaderType;
            shaderCreateInfo.Code = fileContent;

            return renderDevice->CreateShader(shaderCreateInfo);
        };

        Renderer::StaticSamplerInfo staticSamplers[] = { Renderer::StaticSamplerInfo{ "inTexture", Renderer::StaticSampler::NearestClamp } };

        Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
        shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
        shaderProgramCreateInfo.ShaderCount = 2;
        shaderProgramCreateInfo.StaticSamplerCount = 1;
        shaderProgramCreateInfo.StaticSamplers = staticSamplers;
        shaderProgramCreateInfo.Shaders[0] = loadShader(Renderer::ShaderType::Vertex, CK_TEXT("builtin://sponza/resources/Shaders/ImGui/default.vert.spv"));
        shaderProgramCreateInfo.Shaders[1] = loadShader(Renderer::ShaderType::Fragment, CK_TEXT("builtin://sponza/resources/Shaders/ImGui/default.frag.spv"));
        mShaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);

        mTextureUniformSlot = mShaderProgram->FindUniformSlot("inTexture");
        assert(mTextureUniformSlot);
    }

    void WindowRenderer::Draw(ImGuiContext* context, WindowResources* windowResources, Renderer::RenderContext* renderContext, Renderer::Framebuffer* framebuffer) const
    {
        const ImDrawData& drawData = context->Viewports[0]->DrawDataP;
        if (!drawData.Valid)
            return;

        Extent2D<float> framebufferSize;
        framebufferSize.Width = drawData.DisplaySize.x * drawData.FramebufferScale.x;
        framebufferSize.Height = drawData.DisplaySize.y * drawData.FramebufferScale.y;
        if (framebufferSize.Width <= 0.f || framebufferSize.Height <= 0.f)
            return;

        Renderer::CommandList* commandList = renderContext->CreateCommandList({ Renderer::CommandListUsageBits::Graphic });

        commandList->Begin(nullptr);
        commandList->BeginRenderPass({ Renderer::RenderPassMode::Load, framebuffer });

        SetupRenderState(context, windowResources, commandList, framebufferSize);

        for (ImDrawList* drawList : drawData.CmdLists)
        {
            Renderer::BufferAllocator* bufferAllocator = renderContext->GetBufferAllocator(Renderer::BufferUsageFlagBits::Vertex | Renderer::BufferUsageFlagBits::Index, Renderer::MemoryType::Unified);

            Renderer::BufferArea vertexBufferArea = bufferAllocator->PushData(drawList->VtxBuffer.size_in_bytes(), drawList->VtxBuffer.Data);
            Renderer::BufferArea indexBufferArea = bufferAllocator->PushData(drawList->IdxBuffer.size_in_bytes(), drawList->IdxBuffer.Data);

            assert(vertexBufferArea.Range == drawList->VtxBuffer.size_in_bytes());
            assert(indexBufferArea.Range == drawList->IdxBuffer.size_in_bytes());

            commandList->BindVertexBuffer(0, vertexBufferArea.Buffer, vertexBufferArea.BaseOffset, sizeof(ImDrawVert));
            commandList->BindIndexBuffer(indexBufferArea.Buffer, indexBufferArea.BaseOffset, sizeof(ImDrawIdx) == 2 ? Renderer::IndexType::Short : Renderer::IndexType::Integer);

            for (const ImDrawCmd& drawCmd : drawList->CmdBuffer)
            {
                if (drawCmd.UserCallback)
                {
                    if (drawCmd.UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        SetupRenderState(context, windowResources, commandList, framebufferSize);
                    }
                    else
                    {
                        drawCmd.UserCallback(drawList, &drawCmd);
                    }
                }
                else
                {
                    ImVec2 clipOffset = drawData.DisplayPos;
                    ImVec2 clipScale = drawData.FramebufferScale;

                    ImVec2 clipMinBounds((drawCmd.ClipRect.x - clipOffset.x) * clipScale.x, (drawCmd.ClipRect.y - clipOffset.y) * clipScale.y);
                    ImVec2 clipMaxBounds((drawCmd.ClipRect.z - clipOffset.x) * clipScale.x, (drawCmd.ClipRect.w - clipOffset.y) * clipScale.y);

                    clipMinBounds.x = std::max(clipMinBounds.x, 0.0f);
                    clipMinBounds.y = std::max(clipMinBounds.y, 0.0f);
                    clipMaxBounds.x = std::min(clipMaxBounds.x, framebufferSize.Width);
                    clipMaxBounds.y = std::min(clipMaxBounds.y, framebufferSize.Height);

                    if (clipMaxBounds.x <= clipMinBounds.x || clipMaxBounds.y <= clipMinBounds.y)
                        continue;

                    Renderer::Scissor scissor;
                    scissor.Position.Width = static_cast<int32_t>(clipMinBounds.x);
                    scissor.Position.Height = static_cast<int32_t>(clipMinBounds.y);
                    scissor.Size.Width = static_cast<uint32_t>(clipMaxBounds.x - clipMinBounds.x);
                    scissor.Size.Height = static_cast<uint32_t>(clipMaxBounds.y - clipMinBounds.y);
                    commandList->SetScissor(scissor);

                    commandList->DrawIndexed(drawCmd.ElemCount, 1, drawCmd.IdxOffset, drawCmd.VtxOffset, 0);
                }
            }
        }

        commandList->EnableVertexBinding(0, false);
        commandList->EndRenderPass();
        commandList->End();

        renderContext->SubmitCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);
    }

    void WindowRenderer::SetupRenderState(ImGuiContext* context, WindowResources* resources, Renderer::CommandList* commandList, Extent2D<unsigned int> framebufferSize) const
    {
        const ImDrawData& drawData = context->Viewports[0]->DrawDataP;
        assert(drawData.Valid);

        Renderer::VertexInputAttribute inputAttributes[] = {
            { 0, PixelFormat::Color(PixelFormat::Layout::RG, DataType::Float32, PixelFormat::Encoding::Raw), 0 },
            { 1, PixelFormat::Color(PixelFormat::Layout::RG, DataType::Float32, PixelFormat::Encoding::Raw), sizeof(float) * 2 },
            { 2, PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized), sizeof(float) * 4 },
        };
        commandList->EnableVertexBinding(0, true);
        commandList->SetVertexInputAttributes(0, 3, inputAttributes);

        Renderer::Viewport viewport;
        viewport.Width = framebufferSize.Width;
        viewport.Height = framebufferSize.Height;
        commandList->SetViewport(viewport);

        commandList->SetPolygonMode(Renderer::PolygonMode::Fill);
        commandList->SetCullMode(Renderer::CullMode::None);
        commandList->SetFrontFace(Renderer::FrontFace::CounterClockwise);
        commandList->SetLineWidth(1.f);

        commandList->EnableBlending(0, true);

        commandList->BindShaderProgram(mShaderProgram.get());
        commandList->BindTextureSampler(mTextureUniformSlot, 0, resources->GetFontTextureView(), nullptr);

        struct VertexInfo
        {
            Vector2<float> Scale;
            Vector2<float> Translate;
        };

        VertexInfo vertexInfo;
        vertexInfo.Scale.X() = 2.0f / drawData.DisplaySize.x;
        vertexInfo.Scale.Y() = 2.0f / drawData.DisplaySize.y;
        vertexInfo.Translate.X() = -1.0f - drawData.DisplayPos.x * vertexInfo.Scale.X();
        vertexInfo.Translate.Y() = -1.0f - drawData.DisplayPos.y * vertexInfo.Scale.Y();

        commandList->UpdatePipelineConstant(Renderer::ShaderType::Vertex, 0, sizeof(VertexInfo), &vertexInfo);
    }
}
