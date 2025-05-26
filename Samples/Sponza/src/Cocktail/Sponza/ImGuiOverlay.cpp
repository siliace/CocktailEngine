#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/System/Clipboard/ClipboardService.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Buffer/BufferAllocator.hpp>
#include <Cocktail/Renderer/Command/CommandListDynamicState.hpp>
#include <Cocktail/Renderer/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Renderer/Command/Synchronization/Fence.hpp>
#include <Cocktail/Renderer/Command/Synchronization/FenceCreateInfo.hpp>
#include <Cocktail/Renderer/Context/FrameContext.hpp>
#include <Cocktail/Renderer/Shader/DescriptorType.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>
#include <Cocktail/Renderer/Texture/SamplerCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

#include <Cocktail/Sponza/ImGuiOverlay.hpp>
#include <Cocktail/Sponza/ImGui/imgui.h>
#include <Cocktail/Sponza/ImGui/imgui_internal.h>

using namespace Ck;

ImGuiOverlay::ImGuiOverlay(Window& window, SceneViewer& sceneViewer, Renderer::RenderDevice& renderDevice, Renderer::RenderContext& renderContext) :
	mWindow(&window),
	mTextureUniformSlot(nullptr)
{
	ImGuiIO& io = ImGui::GetIO();

	// Create the font texture
	{
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		Renderer::TextureCreateInfo textureCreateInfo;
		textureCreateInfo.Type = Renderer::TextureType::e2D;
		textureCreateInfo.Size = MakeExtent(static_cast<unsigned int>(width), static_cast<unsigned int>(height), 1u);
		textureCreateInfo.Usage = Renderer::TextureUsageFlagBits::Sampled;
		textureCreateInfo.Format = PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8);
		textureCreateInfo.ArrayLayerCount = 1;
		textureCreateInfo.MipMapsCount = 1;
		std::shared_ptr<Renderer::Texture> fontTexture = renderDevice.CreateTexture(textureCreateInfo);

		Renderer::TextureViewCreateInfo textureViewCreateInfo;
		textureViewCreateInfo.Texture = fontTexture;
		textureViewCreateInfo.Type = Renderer::TextureViewType::e2D;
		mFontTextureView = renderDevice.CreateTextureView(textureViewCreateInfo);
		mTextureSampler = renderDevice.CreateSampler(Renderer::SamplerCreateInfo{});

		std::shared_ptr<Renderer::CommandListPool> commandListPool = renderContext.CreateCommandListPool({
			Renderer::CommandListUsage::Transfer
		});

		std::shared_ptr<Renderer::CommandList> commandList = commandListPool->CreateCommandList({
			Renderer::CommandListUsage::Transfer
		});

		Renderer::TextureSubResource fontSubResource = Renderer::TextureSubResource::All(*fontTexture);
		commandList->Begin(nullptr);
		{
			Renderer::GpuBarrier barriers[] = {
				Renderer::GpuBarrier::Of(fontTexture.get(), Renderer::ResourceState::Undefined, Renderer::ResourceState::CopyDestination, fontSubResource)
			};
			commandList->Barrier(1, barriers);
		}

		Renderer::TextureUploadInfo upload{ 0, 0, fontTexture->GetSize(), {0u, 0u, 0u}, pixels };
		commandList->UploadTexture(fontTexture.get(), Renderer::ResourceState::CopyDestination, 1, &upload);

		{
			Renderer::GpuBarrier barriers[] = {
				Renderer::GpuBarrier::Of(fontTexture.get(), Renderer::ResourceState::CopyDestination, Renderer::ResourceState::GraphicShaderResource, fontSubResource)
			};
			commandList->Barrier(1, barriers);
		}

		commandList->End();

		Renderer::FenceCreateInfo fenceCreateInfo;
		std::shared_ptr<Renderer::Fence> fence = renderDevice.CreateFence(fenceCreateInfo);

		Renderer::CommandList* commandLists[] = {
			commandList.get()
		};

		renderContext.ExecuteCommandLists(Renderer::CommandQueueType::Transfer, 1, commandLists, fence.get());
		renderContext.Flush();

		fence->Wait();
	}

	{
		auto loadShader = [&](Renderer::ShaderType shaderType, const std::filesystem::path& path) {
			ByteArray fileContent = FileUtils::ReadFile(path);

			Renderer::ShaderCreateInfo shaderCreateInfo;
			shaderCreateInfo.Type = shaderType;
			shaderCreateInfo.Code = reinterpret_cast<Uint32*>(fileContent.GetData());
			shaderCreateInfo.CodeLength = fileContent.GetSize();

			return renderDevice.CreateShader(shaderCreateInfo);
		};

		Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
		shaderProgramCreateInfo.Type = Renderer::ShaderProgramType::Graphic;
		shaderProgramCreateInfo.ShaderCount = 2;
		shaderProgramCreateInfo.Shaders[0] = loadShader(Renderer::ShaderType::Vertex, "builtin://sponza/resources/Shaders/ImGui/default.vert.spv");
		shaderProgramCreateInfo.Shaders[1] = loadShader(Renderer::ShaderType::Fragment, "builtin://sponza/resources/Shaders/ImGui/default.frag.spv");
		mShaderProgram = renderDevice.CreateShaderProgram(shaderProgramCreateInfo);

		mTextureUniformSlot = mShaderProgram->FindUniformSlot("inTexture");
		assert(mTextureUniformSlot);
	}

	Connect(sceneViewer.OnRendered(), [&](Renderer::RenderContext& rc, Renderer::FrameContext& fc, Renderer::Framebuffer& fb) {
		Render(rc, fc, fb);
	});
}

void ImGuiOverlay::Update()
{
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		ImGui::Text("Cocktail Engine");
		ImGui::EndMainMenuBar();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuiMouseCursor mouseCursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
	if (mouseCursor != mLastMouseCursor)
	{
		mWindow->SetCursor(GetCursor(mouseCursor));
		mLastMouseCursor = mouseCursor;
	}

	ImGui::Render();
}

std::shared_ptr<Cursor> ImGuiOverlay::GetCursor(int mouseCursor)
{
	SystemCursorType cursorType;
	switch (mouseCursor)
	{
	case ImGuiMouseCursor_Arrow:
		cursorType = SystemCursorType::Arrow;
		break;

	case ImGuiMouseCursor_TextInput:
		cursorType = SystemCursorType::Text;
		break;

	case ImGuiMouseCursor_ResizeAll:
		cursorType = SystemCursorType::SizeAll;
		break;

	case ImGuiMouseCursor_ResizeNS:
		cursorType = SystemCursorType::SizeVertical;
		break;

	case ImGuiMouseCursor_ResizeEW:
		cursorType = SystemCursorType::SizeHorizontal;
		break;

	case ImGuiMouseCursor_ResizeNESW:
		cursorType = SystemCursorType::SizeTopLeftBottomRight;
		break;

	case ImGuiMouseCursor_ResizeNWSE:
		cursorType = SystemCursorType::SizeBottomLeftTopRight;
		break;

	case ImGuiMouseCursor_Hand:
		cursorType = SystemCursorType::Hand;
		break;

	case ImGuiMouseCursor_Wait:
		cursorType = SystemCursorType::Wait;
		break;

	case ImGuiMouseCursor_Progress:
		cursorType = SystemCursorType::ArrowWait;
		break;

	case ImGuiMouseCursor_NotAllowed:
		cursorType = SystemCursorType::NotAllowed;
		break;
	}

	return App::Resolve<WindowFactory>()->LoadSystemCursor(cursorType);
}

void ImGuiOverlay::SetupRenderState(Renderer::CommandList* commandList, Extent2D<float> framebufferSize) const
{
	ImDrawData* drawData = ImGui::GetDrawData();

	Renderer::VertexInputAttribute inputAttributes[] = {
		{ 0, PixelFormat::Color(PixelFormat::Layout::RG, DataType::Float32, PixelFormat::Encoding::Raw), 0  },
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
 	commandList->BindTextureSampler(mTextureUniformSlot, 0, mFontTextureView.get(), mTextureSampler.get());

	struct VertexInfo
	{
		Vector2<float> Scale;
		Vector2<float> Translate;
	};

	VertexInfo vertexInfo;
	vertexInfo.Scale.X() = 2.0f / drawData->DisplaySize.x;
	vertexInfo.Scale.Y() = 2.0f / drawData->DisplaySize.y;
	vertexInfo.Translate.X() = -1.0f - drawData->DisplayPos.x * vertexInfo.Scale.X();
	vertexInfo.Translate.Y() = -1.0f - drawData->DisplayPos.y * vertexInfo.Scale.Y();

	commandList->UpdatePipelineConstant(Renderer::ShaderType::Vertex, 0, sizeof(VertexInfo), &vertexInfo);
}

void ImGuiOverlay::Render(Renderer::RenderContext& renderContext, Renderer::FrameContext& frameContext, Renderer::Framebuffer& framebuffer)
{
	ImDrawData* drawData = ImGui::GetDrawData();
	if (!drawData)
		return;

	Extent2D<float> framebufferSize;
	framebufferSize.Width = drawData->DisplaySize.x * drawData->FramebufferScale.x;
	framebufferSize.Height = drawData->DisplaySize.y * drawData->FramebufferScale.y;
	if (framebufferSize.Width <= 0.f || framebufferSize.Height <= 0.f)
		return;

	std::shared_ptr<Renderer::CommandList> commandList = frameContext.CreateCommandList({ Renderer::CommandListUsage::Graphic, Renderer::CommandListDynamicStateBits::Scissor | Renderer::CommandListDynamicStateBits::Viewport });

	commandList->Begin(nullptr);
	commandList->BeginRenderPass({ Renderer::RenderPassMode::Load, &framebuffer });

	SetupRenderState(commandList.get(), framebufferSize);

	for (ImDrawList* drawList : drawData->CmdLists)
	{
		Renderer::BufferAllocator* bufferAllocator = frameContext.GetBufferAllocator(Renderer::BufferUsageFlagBits::Vertex | Renderer::BufferUsageFlagBits::Index, Renderer::MemoryType::Unified);

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
					SetupRenderState(commandList.get(), framebufferSize);
				}
				else
				{
					drawCmd.UserCallback(drawList, &drawCmd);
				}
			}
			else
			{
				ImVec2 clipOffset = drawData->DisplayPos;
				ImVec2 clipScale = drawData->FramebufferScale;

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

	Renderer::CommandList* commandLists[] = {
		commandList.get()
	};

	renderContext.ExecuteCommandLists(Renderer::CommandQueueType::Graphic, 1, commandLists, nullptr);
}
