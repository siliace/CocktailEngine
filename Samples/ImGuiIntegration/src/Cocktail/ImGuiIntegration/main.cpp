#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

#include <Cocktail/ImGuiIntegration/ImGui/imgui.h>

#ifdef _DEBUG
	#define ENABLE_RENDER_DEVICE_DEBUG true
#else
	#define ENABLE_RENDER_DEVICE_DEBUG false
#endif

struct PerFrameInfo
{
	Ck::Ref<Ck::Renderer::Buffer> VertexBuffer;
	Ck::Ref<Ck::Renderer::Buffer> IndexBuffer;
};

class ImGuiHelper
{
public:

	static int GetMouseButtonIndex(Ck::MouseButton button)
	{
		switch (button)
		{
			case Ck::MouseButton::Left: return 0;
			case Ck::MouseButton::Middle: return 2;
			case Ck::MouseButton::Right: return 1;
			case Ck::MouseButton::Extra1: return 3;
			case Ck::MouseButton::Extra2: return 4;
		}

		COCKTAIL_UNREACHABLE();
	}

	static Ck::SystemCursorType GetSystemCursorType(ImGuiMouseCursor cursor)
	{
		switch (cursor)
		{
			case ImGuiMouseCursor_Arrow: return Ck::SystemCursorType::Arrow;
			case ImGuiMouseCursor_TextInput: return Ck::SystemCursorType::Text;
			case ImGuiMouseCursor_ResizeAll: return Ck::SystemCursorType::SizeAll;
			case ImGuiMouseCursor_ResizeNS: return Ck::SystemCursorType::SizeVertical;
			case ImGuiMouseCursor_ResizeEW: return Ck::SystemCursorType::SizeHorizontal;
			case ImGuiMouseCursor_ResizeNESW: return Ck::SystemCursorType::SizeTopLeftBottomRight;
			case ImGuiMouseCursor_ResizeNWSE: return Ck::SystemCursorType::SizeBottomLeftTopRight;
			case ImGuiMouseCursor_Hand:	return Ck::SystemCursorType::Hand;
			case ImGuiMouseCursor_NotAllowed: return Ck::SystemCursorType::NotAllowed;
		}

		COCKTAIL_UNREACHABLE();
	}

	static ImGuiKey GetKeyCode(Ck::KeyboardKey key)
	{
		switch (key)
		{
			case Ck::KeyboardKey::Backspace: return ImGuiKey_Backspace;
			case Ck::KeyboardKey::Tabulation: return ImGuiKey_Tab;
			case Ck::KeyboardKey::Clear: return ImGuiKey_Delete;
			case Ck::KeyboardKey::Enter: return ImGuiKey_Enter;
			case Ck::KeyboardKey::Pause: return ImGuiKey_Pause;
			case Ck::KeyboardKey::CapsLock: return ImGuiKey_CapsLock;
			case Ck::KeyboardKey::Escape: return ImGuiKey_Escape;
			case Ck::KeyboardKey::Space: return ImGuiKey_Space;
			case Ck::KeyboardKey::Prior: return ImGuiKey_PageUp;
			case Ck::KeyboardKey::Next: return ImGuiKey_PageDown;
			case Ck::KeyboardKey::End: return ImGuiKey_End;
			case Ck::KeyboardKey::Home: return ImGuiKey_Home;
			case Ck::KeyboardKey::Left: return ImGuiKey_LeftArrow;
			case Ck::KeyboardKey::Up: return ImGuiKey_UpArrow;
			case Ck::KeyboardKey::Right: return ImGuiKey_RightArrow;
			case Ck::KeyboardKey::Down: return ImGuiKey_DownArrow;
			case Ck::KeyboardKey::ScreenShot: return ImGuiKey_PrintScreen;
			case Ck::KeyboardKey::Insert: return ImGuiKey_Insert;
			case Ck::KeyboardKey::Delete: return ImGuiKey_Delete;
			case Ck::KeyboardKey::A: return ImGuiKey_A;
			case Ck::KeyboardKey::B: return ImGuiKey_B;
			case Ck::KeyboardKey::C: return ImGuiKey_C;
			case Ck::KeyboardKey::D: return ImGuiKey_D;
			case Ck::KeyboardKey::E: return ImGuiKey_E;
			case Ck::KeyboardKey::F: return ImGuiKey_F;
			case Ck::KeyboardKey::G: return ImGuiKey_G;
			case Ck::KeyboardKey::H: return ImGuiKey_H;
			case Ck::KeyboardKey::I: return ImGuiKey_I;
			case Ck::KeyboardKey::J: return ImGuiKey_J;
			case Ck::KeyboardKey::K: return ImGuiKey_K;
			case Ck::KeyboardKey::L: return ImGuiKey_L;
			case Ck::KeyboardKey::M: return ImGuiKey_M;
			case Ck::KeyboardKey::N: return ImGuiKey_N;
			case Ck::KeyboardKey::O: return ImGuiKey_O;
			case Ck::KeyboardKey::P: return ImGuiKey_P;
			case Ck::KeyboardKey::Q: return ImGuiKey_Q;
			case Ck::KeyboardKey::R: return ImGuiKey_R;
			case Ck::KeyboardKey::S: return ImGuiKey_S;
			case Ck::KeyboardKey::T: return ImGuiKey_T;
			case Ck::KeyboardKey::U: return ImGuiKey_U;
			case Ck::KeyboardKey::V: return ImGuiKey_V;
			case Ck::KeyboardKey::W: return ImGuiKey_W;
			case Ck::KeyboardKey::X: return ImGuiKey_X;
			case Ck::KeyboardKey::Y: return ImGuiKey_Y;
			case Ck::KeyboardKey::Z: return ImGuiKey_Z;
			case Ck::KeyboardKey::LeftSystem: return ImGuiKey_LeftSuper;
			case Ck::KeyboardKey::RightSystem: return ImGuiKey_RightSuper;
			case Ck::KeyboardKey::App: return ImGuiKey_Menu;
			case Ck::KeyboardKey::NumPad0: return ImGuiKey_Keypad0;
			case Ck::KeyboardKey::NumPad1: return ImGuiKey_Keypad1;
			case Ck::KeyboardKey::NumPad2: return ImGuiKey_Keypad2;
			case Ck::KeyboardKey::NumPad3: return ImGuiKey_Keypad3;
			case Ck::KeyboardKey::NumPad4: return ImGuiKey_Keypad4;
			case Ck::KeyboardKey::NumPad5: return ImGuiKey_Keypad5;
			case Ck::KeyboardKey::NumPad6: return ImGuiKey_Keypad6;
			case Ck::KeyboardKey::NumPad7: return ImGuiKey_Keypad7;
			case Ck::KeyboardKey::NumPad8: return ImGuiKey_Keypad8;
			case Ck::KeyboardKey::NumPad9: return ImGuiKey_Keypad9;
			case Ck::KeyboardKey::Multiply: return ImGuiKey_KeypadMultiply;
			case Ck::KeyboardKey::Add: return ImGuiKey_KeypadAdd;
			case Ck::KeyboardKey::Subtract: return ImGuiKey_KeypadSubtract;
			case Ck::KeyboardKey::Decimal: return ImGuiKey_KeypadDecimal;
			case Ck::KeyboardKey::Divide: return ImGuiKey_KeypadDivide;
			case Ck::KeyboardKey::F1: return ImGuiKey_F1;
			case Ck::KeyboardKey::F2: return ImGuiKey_F2;
			case Ck::KeyboardKey::F3: return ImGuiKey_F3;
			case Ck::KeyboardKey::F4: return ImGuiKey_F4;
			case Ck::KeyboardKey::F5: return ImGuiKey_F5;
			case Ck::KeyboardKey::F6: return ImGuiKey_F6;
			case Ck::KeyboardKey::F7: return ImGuiKey_F7;
			case Ck::KeyboardKey::F8: return ImGuiKey_F8;
			case Ck::KeyboardKey::F9: return ImGuiKey_F9;
			case Ck::KeyboardKey::F10: return ImGuiKey_F10;
			case Ck::KeyboardKey::F11: return ImGuiKey_F11;
			case Ck::KeyboardKey::F12: return ImGuiKey_F12;
			case Ck::KeyboardKey::F13: return ImGuiKey_F13;
			case Ck::KeyboardKey::F14: return ImGuiKey_F14;
			case Ck::KeyboardKey::F15: return ImGuiKey_F15;
			case Ck::KeyboardKey::F16: return ImGuiKey_F16;
			case Ck::KeyboardKey::F17: return ImGuiKey_F17;
			case Ck::KeyboardKey::F18: return ImGuiKey_F18;
			case Ck::KeyboardKey::F19: return ImGuiKey_F19;
			case Ck::KeyboardKey::F20: return ImGuiKey_F20;
			case Ck::KeyboardKey::F21: return ImGuiKey_F21;
			case Ck::KeyboardKey::F22: return ImGuiKey_F22;
			case Ck::KeyboardKey::F23: return ImGuiKey_F23;
			case Ck::KeyboardKey::F24: return ImGuiKey_F24;
			case Ck::KeyboardKey::NumLock: return ImGuiKey_NumLock;
			case Ck::KeyboardKey::ScrollLock: return ImGuiKey_ScrollLock;
			case Ck::KeyboardKey::LeftShift: return ImGuiKey_LeftShift;
			case Ck::KeyboardKey::RightShift: return ImGuiKey_RightShift;
			case Ck::KeyboardKey::LeftControl: return ImGuiKey_LeftCtrl;
			case Ck::KeyboardKey::RightControl: return ImGuiKey_RightCtrl;
			case Ck::KeyboardKey::LeftAlt: return ImGuiKey_LeftAlt;
			case Ck::KeyboardKey::RightAlt: return ImGuiKey_RightAlt;
			default: return ImGuiKey_None;
		}
	}
};

Ck::Ref<Ck::Renderer::TextureView> CreateFontTextureView(Ck::Renderer::RenderContext& renderContext, ImGuiIO& io)
{
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

	Ck::Ref<Ck::Renderer::RenderDevice> renderDevice = renderContext.GetRenderDevice();

	Ck::Renderer::TextureCreateInfo textureCreateInfo;
	textureCreateInfo.Type = Ck::Renderer::TextureType::e2D;
	textureCreateInfo.Size = Ck::MakeExtent<unsigned int>(width, height, 1);
	textureCreateInfo.Format = Ck::PixelFormat::Color(Ck::PixelFormat::Layout::R, Ck::DataType::UnsignedInt8);
	Ck::Ref<Ck::Renderer::Texture> fontTexture = renderDevice->CreateTexture(textureCreateInfo);

	Ck::Renderer::TextureViewCreateInfo textureViewCreateInfo;
	textureViewCreateInfo.Type = Ck::Renderer::TextureViewType::e2D;
	textureViewCreateInfo.Texture = fontTexture;
	textureViewCreateInfo.Swizzle.Red = Ck::Renderer::TextureSwizzleValue::Red;
	textureViewCreateInfo.Swizzle.Green = Ck::Renderer::TextureSwizzleValue::Red;
	textureViewCreateInfo.Swizzle.Blue = Ck::Renderer::TextureSwizzleValue::Red;
	textureViewCreateInfo.Swizzle.Alpha = Ck::Renderer::TextureSwizzleValue::Red;
	Ck::Ref<Ck::Renderer::TextureView> fontTextureView = renderDevice->CreateTextureView(textureViewCreateInfo);

	Ck::Ref<Ck::Renderer::Fence> fence = renderDevice->CreateFence({});
	Ck::Ref<Ck::Renderer::CommandListPool> commandListPool = renderContext.CreateCommandListPool({ Ck::Renderer::CommandListUsage::Transfer, true, false });

	Ck::Ref<Ck::Renderer::CommandList> transferCommandList = commandListPool->CreateCommandList({
		Ck::Renderer::CommandListUsage::Transfer
	});

	Ck::Renderer::TextureUploadInfo uploadInfo;
	uploadInfo.Size = fontTexture->GetSize();
	uploadInfo.Pixels = pixels;

	transferCommandList->Begin();
	Ck::Renderer::GpuBarrier preCopyBarriers = Ck::Renderer::GpuBarrier::Of(
		fontTexture.Get(), 
		Ck::Renderer::ResourceState::Undefined, 
		Ck::Renderer::ResourceState::CopyDestination,
		Ck::Renderer::TextureSubResource::All(*fontTexture)
	);
	transferCommandList->Barrier(1, &preCopyBarriers);

	transferCommandList->UploadTexture(fontTexture.Get(), Ck::Renderer::ResourceState::CopyDestination, 1, &uploadInfo);

	Ck::Renderer::GpuBarrier postCopyBarriers = Ck::Renderer::GpuBarrier::Of(
		fontTexture.Get(),
		Ck::Renderer::ResourceState::CopyDestination,
		Ck::Renderer::ResourceState::GraphicShaderResource,
		Ck::Renderer::TextureSubResource::All(*fontTexture)
	);
	transferCommandList->Barrier(1, &postCopyBarriers);
	transferCommandList->End();

	renderContext.ExecuteCommandLists(Ck::Renderer::CommandQueueType::Transfer, 1, &transferCommandList, fence);
	renderContext.SignalQueue(Ck::Renderer::CommandQueueType::Transfer);
	renderContext.WaitQueue(Ck::Renderer::CommandQueueType::Graphic, Ck::Renderer::CommandQueueType::Transfer);
	renderContext.Flush();

	fence->Wait();

	return fontTextureView;
}

Ck::Ref<Ck::Renderer::Shader> LoadShaderFromPath(Ck::Renderer::RenderDevice& renderDevice, const std::filesystem::path& path, Ck::Renderer::ShaderType shaderType)
{
	std::string code = Ck::FileUtils::ReadFile(path);

	Ck::Renderer::ShaderCreateInfo shaderCreateInfo;
	shaderCreateInfo.Type = shaderType;
	shaderCreateInfo.Code = reinterpret_cast<Ck::Uint32*>(code.data());
	shaderCreateInfo.CodeLength = code.size();

	return renderDevice.CreateShader(shaderCreateInfo);
}

void SetupRenderState(ImDrawData* drawData, Ck::Renderer::CommandList& commandList, Ck::Renderer::ShaderProgram* shaderProgram, Ck::Renderer::Buffer* vertexBuffer, Ck::Renderer::Buffer* indexBuffer)
{
	Ck::Renderer::UniformSlot* vertexPipelineConstantSlot;
	shaderProgram->GetUniformSlots(&vertexPipelineConstantSlot, 1, 1);
	assert(vertexPipelineConstantSlot->GetDescriptorType() == Ck::Renderer::DescriptorType::PipelineConstants);

	float scale[2];
	scale[0] = 2.0f / drawData->DisplaySize.x;
	scale[1] = 2.0f / drawData->DisplaySize.y;

	float translate[2];
	translate[0] = -1.0f - drawData->DisplayPos.x * scale[0];
	translate[1] = -1.0f - drawData->DisplayPos.y * scale[1];

	commandList.BindShaderProgram(shaderProgram);
	commandList.BindVertexBuffer(0, vertexBuffer, 0, sizeof(ImDrawVert));
	commandList.BindIndexBuffer(indexBuffer, 0, Ck::Renderer::IndexType::Short);
	commandList.UpdatePipelineConstant(vertexPipelineConstantSlot, 0, sizeof scale, scale);
	commandList.UpdatePipelineConstant(vertexPipelineConstantSlot, sizeof scale, sizeof translate, translate);

	Ck::Renderer::VertexInputAttribute vertexInputAttributes[] = {
		{ 0, Ck::PixelFormat::Color(Ck::PixelFormat::Layout::RG, Ck::DataType::Float32, Ck::PixelFormat::Encoding::Raw), 0 },
		{ 1, Ck::PixelFormat::Color(Ck::PixelFormat::Layout::RG, Ck::DataType::Float32, Ck::PixelFormat::Encoding::Raw), 2 * sizeof(float) },
		{ 2, Ck::PixelFormat::Color(Ck::PixelFormat::Layout::RGBA, Ck::DataType::UnsignedInt8, Ck::PixelFormat::Encoding::Normalized), 4 * sizeof(float) },
	};

	commandList.EnableVertexBinding(0, true);
	commandList.SetVertexInputAttributes(0, 3, vertexInputAttributes);
	commandList.SetCullMode(Ck::Renderer::CullMode::None);
	commandList.EnableBlending(0, true);
	commandList.SetBlendingEquation(0, Ck::Renderer::BlendOp::Add, Ck::Renderer::BlendOp::Add);
	commandList.SetBlendingFunction(
		0, 
		Ck::Renderer::BlendFactor::SourceAlpha, Ck::Renderer::BlendFactor::OneMinusSourceAlpha, 
		Ck::Renderer::BlendFactor::One, Ck::Renderer::BlendFactor::OneMinusSourceAlpha
	);
}

Ck::Main::ExitCode ApplicationMain(Ck::Ref<Ck::Application> application)
{
	application->Invoke([](Ck::Ref<Ck::LogManager> logManager, Ck::Ref<Ck::ConsoleLogChannel> consoleLogChannel) {
		logManager->RegisterChannel("default", std::move(consoleLogChannel));
	});

	Ck::WindowCreateInfo windowCreateInfo;
	windowCreateInfo.Size = Ck::MakeExtent(800u, 600u);
	windowCreateInfo.Title = "ImGui Integration";
	windowCreateInfo.Position = Ck::MakeExtent(100, 100);
	windowCreateInfo.Style = Ck::WindowStyleFlags::All();
	Ck::Ref<Ck::Window> window = application->Invoke([&](const Ck::Ref<Ck::WindowFactory>& windowFactory) {
		return windowFactory->CreateWindow(windowCreateInfo);
	});

	Ck::Ref<Ck::Renderer::RenderDevice> renderDevice = application->Invoke([&](const Ck::Ref<Ck::Renderer::RendererService>& rendererService) {
		return rendererService->CreateRenderDevice(Ck::Renderer::GraphicApi::Vulkan, ENABLE_RENDER_DEVICE_DEBUG);
	});

	application->Connect(renderDevice->OnDebugMessage(), [](std::string_view message) {
		Ck::Log::Info("Debug message from RenderDevice : {}", message);
	});

	Ck::Renderer::UniformSlot* textureSlot;
	Ck::Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
	shaderProgramCreateInfo.Type = Ck::Renderer::ShaderProgramType::Graphic;
	shaderProgramCreateInfo.ShaderCount = 2;
	shaderProgramCreateInfo.Shaders[0] = LoadShaderFromPath(*renderDevice, "./resources/shaders/imgui.vert.spv", Ck::Renderer::ShaderType::Vertex);
	shaderProgramCreateInfo.Shaders[1] = LoadShaderFromPath(*renderDevice, "./resources/shaders/imgui.frag.spv", Ck::Renderer::ShaderType::Fragment);
	Ck::Ref<Ck::Renderer::ShaderProgram> shaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);
	shaderProgram->GetUniformSlots(&textureSlot, 1, 0);
	assert(textureSlot->GetDescriptorType() == Ck::Renderer::DescriptorType::TextureSampler);

	Ck::Renderer::RenderSurfaceCreateInfo renderSurfaceCreateInfo;
	renderSurfaceCreateInfo.Window = window;
	renderSurfaceCreateInfo.DepthStencilFormat = Ck::PixelFormat::DepthStencil(24, 8);
	renderSurfaceCreateInfo.Samples = Ck::Renderer::RasterizationSamples::e4;
	Ck::Ref<Ck::Renderer::RenderSurface> renderSurface = renderDevice->CreateRenderSurface(renderSurfaceCreateInfo);

	Ck::Renderer::RenderContextCreateInfo renderContextCreateInfo;
	renderContextCreateInfo.RenderSurface = renderSurface;
	Ck::Ref<Ck::Renderer::RenderContext> renderContext = renderDevice->CreateRenderContext(renderContextCreateInfo);

	std::unordered_map<Ck::Renderer::FrameToken, PerFrameInfo> perFramesInfo;

	ImGui::CreateContext();
	ImGui::GetMainViewport()->PlatformHandleRaw = window->GetSystemHandle();

	ImGuiIO& io = ImGui::GetIO();
	Ck::Ref<Ck::Renderer::TextureView> fontView = CreateFontTextureView(*renderContext, io);
	Ck::Ref<Ck::Renderer::Sampler> fontSampler = renderDevice->CreateSampler({});

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;
	io.DisplaySize.x = window->GetSize().Width;
	io.DisplaySize.y = window->GetSize().Height;

	application->Connect(window->OnResizedEvent(), [&](const Ck::WindowResizedEvent& event) {
		io.DisplaySize.x = event.Size.Width;
		io.DisplaySize.y = event.Size.Height;
	});

	application->Connect(window->OnMouseButtonEvent(), [&](const Ck::WindowMouseButtonEvent& event) {
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMouseButtonEvent(ImGuiHelper::GetMouseButtonIndex(event.Button), event.Type == Ck::MouseButtonEvent::ClickType::Pressed);
	});

	application->Connect(window->OnMouseMovedEvent(), [&](const Ck::WindowMouseMoveEvent& event) {
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMousePosEvent(event.Position.Width, event.Position.Height);
	});

	application->Connect(window->OnMouseWheelEvent(), [&](const Ck::WindowMouseWheelEvent& event) {
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		if (event.Wheel == Ck::MouseWheel::Vertical)
		{
			io.AddMouseWheelEvent(0.f, event.Delta);
		}
		else
		{
			io.AddMouseWheelEvent(event.Delta, 0.f);
		}
	});

	application->Connect(window->OnFocusEvent(), [&](const Ck::WindowFocusEvent& event) {
		io.AddFocusEvent(event.HasFocus);
	});

	application->Connect(window->OnKeyboardEvent(), [&](const Ck::WindowKeyboardEvent& event) {
		io.AddKeyEvent(ImGuiHelper::GetKeyCode(event.Key), event.Pressed);
	});

	application->Connect(window->OnTextEvent(), [&](const Ck::WindowTextEvent& event) {
		io.AddInputCharacter(event.Unicode);
	});

	application->Connect(renderContext->OnBeforeRedraw(), [&](Ck::Renderer::FrameContext* frameContext) {
		ImDrawData* drawData = ImGui::GetDrawData();	PerFrameInfo& perFrameInfo = perFramesInfo[frameContext->GetToken()];

		if (!drawData || drawData->TotalVtxCount == 0 || drawData->TotalIdxCount == 0)
			return;

		if (!perFrameInfo.VertexBuffer || perFrameInfo.VertexBuffer->GetSize() < drawData->TotalVtxCount * sizeof(ImDrawVert))
		{
			Ck::Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Usage = Ck::Renderer::BufferUsageFlagBits::Vertex;
			bufferCreateInfo.Size = drawData->TotalVtxCount * sizeof(ImDrawVert);
			perFrameInfo.VertexBuffer = renderDevice->CreateBuffer(bufferCreateInfo);
		}

		if (!perFrameInfo.IndexBuffer || perFrameInfo.IndexBuffer->GetSize() < drawData->TotalIdxCount * sizeof(ImDrawIdx))
		{
			Ck::Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Usage = Ck::Renderer::BufferUsageFlagBits::Index;
			bufferCreateInfo.Size = drawData->TotalIdxCount * sizeof(ImDrawIdx);
			perFrameInfo.IndexBuffer = renderDevice->CreateBuffer(bufferCreateInfo);
		}

		std::size_t vertexOffset = 0;
		std::size_t indexOffset = 0;
		Ck::Renderer::BufferUploadInfo* vertexBufferUploads = COCKTAIL_STACK_ALLOC(Ck::Renderer::BufferUploadInfo, drawData->CmdListsCount);
		Ck::Renderer::BufferUploadInfo* indexBufferUploads = COCKTAIL_STACK_ALLOC(Ck::Renderer::BufferUploadInfo, drawData->CmdListsCount);
		for (int i = 0; i < drawData->CmdListsCount; i++)
		{
			ImDrawList* drawList = drawData->CmdLists[i];

			vertexBufferUploads[i].Offset = vertexOffset;
			vertexBufferUploads[i].Length = drawList->VtxBuffer.size() * sizeof(ImDrawVert);
			vertexBufferUploads[i].Data = drawList->VtxBuffer.Data;

			indexBufferUploads[i].Offset = indexOffset;
			indexBufferUploads[i].Length = drawList->IdxBuffer.size() * sizeof(ImDrawIdx);
			indexBufferUploads[i].Data = drawList->IdxBuffer.Data;

			vertexOffset += vertexBufferUploads[i].Length;
			indexOffset += indexBufferUploads[i].Length;
		}

		Ck::Ref<Ck::Renderer::CommandList> commandList = frameContext->CreateCommandList({ Ck::Renderer::CommandListUsage::Transfer });

		commandList->Begin();
		commandList->UploadBuffer(perFrameInfo.VertexBuffer.Get(), drawData->CmdListsCount, vertexBufferUploads);
		commandList->UploadBuffer(perFrameInfo.IndexBuffer.Get(), drawData->CmdListsCount, indexBufferUploads);
		commandList->End();

		renderContext->ExecuteCommandLists(Ck::Renderer::CommandQueueType::Transfer, 1, &commandList, nullptr);
		renderContext->SignalQueue(Ck::Renderer::CommandQueueType::Transfer);
		renderContext->WaitQueue(Ck::Renderer::CommandQueueType::Graphic, Ck::Renderer::CommandQueueType::Transfer);
	});

	application->Connect(renderContext->OnRedraw(), [&](Ck::Renderer::FrameContext* frameContext, const Ck::Renderer::Framebuffer* framebuffer) {
		ImDrawData* drawData = ImGui::GetDrawData();
		PerFrameInfo& perFrameInfo = perFramesInfo[frameContext->GetToken()];

		Ck::Renderer::CommandListCreateInfo commandListCreateInfo;
		commandListCreateInfo.Usage = Ck::Renderer::CommandListUsage::Graphic;
		commandListCreateInfo.DynamicState = Ck::Renderer::CommandListDynamicState::Of(Ck::Renderer::CommandListDynamicStateBits::Viewport, Ck::Renderer::CommandListDynamicStateBits::Scissor);
		Ck::Ref<Ck::Renderer::CommandList> commandList = frameContext->CreateCommandList(commandListCreateInfo);

		commandList->Begin();
		commandList->BeginRenderPass({ Ck::Renderer::RenderPassMode::Clear, framebuffer });

		if (drawData && perFrameInfo.VertexBuffer && perFrameInfo.IndexBuffer)
		{
			float framebufferWidth = drawData->DisplaySize.x * drawData->FramebufferScale.x;
			float framebufferHeight = drawData->DisplaySize.y * drawData->FramebufferScale.y;

			SetupRenderState(drawData, *commandList, shaderProgram.Get(), perFrameInfo.VertexBuffer.Get(), perFrameInfo.IndexBuffer.Get());

			std::size_t globalVertexOffset = 0;
			std::size_t globalIndexOffset = 0;
			for (ImDrawList* drawList : drawData->CmdLists)
			{
				for (const ImDrawCmd& drawCommand : drawList->CmdBuffer)
				{
					if (drawCommand.UserCallback)
					{
						if (drawCommand.UserCallback == ImDrawCallback_ResetRenderState)
						{
							SetupRenderState(drawData, *commandList, shaderProgram.Get(), perFrameInfo.VertexBuffer.Get(), perFrameInfo.IndexBuffer.Get());
						}
						else
						{
							drawCommand.UserCallback(drawList, &drawCommand);
						}
					}
					else
					{
						ImVec4 clip = drawCommand.ClipRect;
						clip.x = std::max(0.f, clip.x);
						clip.y = std::max(0.f, clip.y);
						clip.z = std::min(framebufferWidth, clip.z);
						clip.w = std::min(framebufferHeight, clip.w);

						Ck::Renderer::Viewport viewport;
						viewport.Width = framebufferWidth;
						viewport.Height = framebufferHeight;
						commandList->SetViewport(viewport);

						Ck::Renderer::Scissor scissor;
						scissor.Position = Ck::MakeExtent<int>(clip.x, clip.y);
						scissor.Size = Ck::MakeExtent<unsigned int>(clip.z - clip.x, clip.w - clip.y);
						commandList->SetScissor(scissor);

						commandList->BindTextureSampler(textureSlot, 0, fontView.Get(), fontSampler.Get());
						commandList->DrawIndexed(drawCommand.ElemCount, 1, drawCommand.IdxOffset + globalIndexOffset, drawCommand.VtxOffset + globalVertexOffset, 0);
					}
				}

				globalVertexOffset += drawList->VtxBuffer.size();
				globalIndexOffset += drawList->IdxBuffer.size();
			}
		}

		commandList->EndRenderPass();
		commandList->End();

		renderContext->ExecuteCommandLists(Ck::Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);
	});

	while (window->PollEvents())
	{
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();
		if (!(io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
		{
			ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
			if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None)
			{
				Ck::Mouse::SetCursorVisible(false);
				window->SetCursor(nullptr);
			}
			else
			{
				Ck::Mouse::SetCursorVisible(true);
				application->Invoke([&](const Ck::Ref<Ck::WindowFactory>& windowFactory) {
					window->SetCursor(windowFactory->LoadSystemCursor(
						ImGuiHelper::GetSystemCursorType(cursor)
					));
				});
			}
		}

		ImGui::Render();

		window->Update();
	}

	ImGui::DestroyContext();

	return Ck::Main::ExitCode::Success;
}
