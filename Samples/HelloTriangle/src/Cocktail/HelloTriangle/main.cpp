#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

#ifdef _DEBUG
	#define ENABLE_RENDER_DEVICE_DEBUG true
#else
	#define ENABLE_RENDER_DEVICE_DEBUG false
#endif

Ck::Ref<Ck::Renderer::Shader> LoadShaderFromPath(Ck::Renderer::RenderDevice& renderDevice, const std::filesystem::path& path, Ck::Renderer::ShaderType shaderType)
{
	std::string code = Ck::FileUtils::ReadFile(path);

	Ck::Renderer::ShaderCreateInfo shaderCreateInfo;
	shaderCreateInfo.Type = shaderType;
	shaderCreateInfo.Code = reinterpret_cast<Ck::Uint32*>(code.data());
	shaderCreateInfo.CodeLength = code.size();

	return renderDevice.CreateShader(shaderCreateInfo);
}

Ck::Main::ExitCode ApplicationMain(Ck::Ref<Ck::Application> application)
{
	application->Invoke([](Ck::Ref<Ck::LogManager> logManager, Ck::Ref<Ck::ConsoleLogChannel> consoleLogChannel) {
		logManager->RegisterChannel("default", std::move(consoleLogChannel));
	});

	Ck::WindowCreateInfo windowCreateInfo;
	windowCreateInfo.Size = Ck::MakeExtent(800u, 600u);
	windowCreateInfo.Title = "Hello Triangle";
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

	float triangleVertices[] = {
		0.0f, -0.5f, 0.f, 1.f, 0.f, 0.f, 1.f,
		0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f,
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 1.f, 1.f
	};

	Ck::Renderer::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.Usage = Ck::Renderer::BufferUsageFlagBits::Vertex;
	bufferCreateInfo.Size = sizeof(triangleVertices);
	Ck::Ref<Ck::Renderer::Buffer> vertexBuffer = renderDevice->CreateBuffer(bufferCreateInfo);

	Ck::Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
	shaderProgramCreateInfo.Type = Ck::Renderer::ShaderProgramType::Graphic;
	shaderProgramCreateInfo.ShaderCount = 2;
	shaderProgramCreateInfo.Shaders[0] = LoadShaderFromPath(*renderDevice, "./resources/shaders/default.vert.spv", Ck::Renderer::ShaderType::Vertex);
	shaderProgramCreateInfo.Shaders[1] = LoadShaderFromPath(*renderDevice, "./resources/shaders/default.frag.spv", Ck::Renderer::ShaderType::Fragment);
	Ck::Ref<Ck::Renderer::ShaderProgram> shaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);

	Ck::Renderer::RenderSurfaceCreateInfo renderSurfaceCreateInfo;
	renderSurfaceCreateInfo.Window = window;
	renderSurfaceCreateInfo.DepthStencilFormat = Ck::PixelFormat::DepthStencil(32, 0);
	renderSurfaceCreateInfo.Samples = Ck::Renderer::RasterizationSamples::e4;
	Ck::Ref<Ck::Renderer::RenderSurface> renderSurface = renderDevice->CreateRenderSurface(renderSurfaceCreateInfo);

	Ck::Renderer::RenderContextCreateInfo renderContextCreateInfo;
	renderContextCreateInfo.RenderSurface = renderSurface;
	Ck::Ref<Ck::Renderer::RenderContext> renderContext = renderDevice->CreateRenderContext(renderContextCreateInfo);

	application->Connect(renderContext->OnBeforeRedraw(), [&](Ck::Renderer::FrameContext* frameContext) {
		Ck::Ref<Ck::Renderer::CommandList> commandList = frameContext->CreateCommandList({ Ck::Renderer::CommandListUsage::Transfer	});

		commandList->Begin();
		commandList->UpdateBuffer(vertexBuffer.Get(), 0, sizeof(triangleVertices), triangleVertices);
		commandList->End();

		renderContext->ExecuteCommandLists(Ck::Renderer::CommandQueueType::Transfer, 1, &commandList, nullptr);
		renderContext->SignalQueue(Ck::Renderer::CommandQueueType::Transfer);
		renderContext->WaitQueue(Ck::Renderer::CommandQueueType::Graphic, Ck::Renderer::CommandQueueType::Transfer);
	});

	application->Connect(renderContext->OnRedraw(), [&](Ck::Renderer::FrameContext* frameContext, const Ck::Renderer::Framebuffer* framebuffer) {
		Ck::Ref<Ck::Renderer::CommandList> commandList = frameContext->CreateCommandList({ Ck::Renderer::CommandListUsage::Graphic });

		commandList->Begin();
		commandList->BeginRenderPass({ Ck::Renderer::RenderPassMode::Clear, framebuffer });

		commandList->BindShaderProgram(shaderProgram.Get());
		commandList->BindVertexBuffer(0, vertexBuffer.Get(), 0, sizeof(float) * 7);

		Ck::Renderer::VertexInputAttribute vertexInputAttributes[] = {
			{ 0, Ck::PixelFormat::Color(Ck::PixelFormat::Layout::RGB, Ck::DataType::Float32, Ck::PixelFormat::Encoding::Raw), 0 },
			{ 1, Ck::PixelFormat::Color(Ck::PixelFormat::Layout::RGBA, Ck::DataType::Float32, Ck::PixelFormat::Encoding::Raw), 3 * sizeof(float) },
		};

		commandList->EnableVertexBinding(0, true);
		commandList->SetVertexInputAttributes(0, 2, vertexInputAttributes);
		commandList->SetCullMode(Ck::Renderer::CullMode::None);

		commandList->Draw(3, 1);

		commandList->EndRenderPass();
		commandList->End();

		renderContext->ExecuteCommandLists(Ck::Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);
	});

	while (window->PollEvents())
		window->Update();

	return Ck::Main::ExitCode::Success;
}
