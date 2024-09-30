#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>
#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>
#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

#include <Cocktail/Vulkan/Vulkan.hpp>

CMRC_DECLARE(hello_triangle);

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
	Ck::Vulkan::GetSupportedApiVersion();

	application->Invoke([](Ck::Ref<Ck::EmbeddedFileSystemDriver> embeddedDriver) {
		embeddedDriver->Register(cmrc::hello_triangle::get_filesystem());
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

	application->Connect(renderDevice->OnDebugMessage(), [](Ck::LogLevel logLevel, Ck::Renderer::MessageType messageType, std::string_view message) {
		Ck::Log::Trace(logLevel, "Debug message of type {} from RenderDevice : {}", Ck::Enum<Ck::Renderer::MessageType>::ToString(messageType), message);
	});

	Ck::Vector3<float> positions[] = {
		Ck::Vector3<float>(0.0f, -0.5f, 0.5f),
		Ck::Vector3<float>(-0.5f, 0.5f, 0.5f),
		Ck::Vector3<float>(0.5f, 0.5f, 0.5f),
	};

	Ck::Vector4<float> colors[] = {
		Ck::Vector4<float>(0.f, 0.f, 1.f, 1.f),
		Ck::Vector4<float>(0.f, 1.f, 0.f, 1.f),
		Ck::Vector4<float>(1.f, 0.f, 0.f, 1.f),
	};

	Ck::VertexLayout::Builder vertexLayoutBuilder;
	vertexLayoutBuilder.AddAttribute(Ck::VertexAttributeSemantic::Position, Ck::DataType::Float32, 3);
	vertexLayoutBuilder.AddAttribute(Ck::VertexAttributeSemantic::Color, Ck::DataType::Float32, 4);
	Ck::Ref<Ck::VertexLayout> vertexLayout = vertexLayoutBuilder.Get();

	Ck::Ref<Ck::VertexArray> vertices = Ck::VertexArray::New(vertexLayout, 3);
	for (std::size_t i = 0; i < vertices->GetVertexCount(); i++)
	{
		Ck::VertexRef vertexRef = vertices->At(i);
		vertexRef.Set(Ck::VertexAttributeSemantic::Position, positions[i]);
		vertexRef.Set(Ck::VertexAttributeSemantic::Color, colors[i]);
	}

	Ck::Ref<Ck::IndexArray> indices = Ck::IndexArray::New(Ck::Renderer::IndexType::Short, 3);
	for (std::size_t i = 0; i < indices->GetIndexCount(); i++)
		indices->At(i).Set(static_cast<unsigned short>(i));

	Ck::Renderer::BufferCreateInfo vertexBufferCreateInfo;
	vertexBufferCreateInfo.Usage = Ck::Renderer::BufferUsageFlagBits::Vertex;
	vertexBufferCreateInfo.Size = vertices->GetVertexCount() * vertexLayout->GetStride();
	Ck::Ref<Ck::Renderer::Buffer> vertexBuffer = renderDevice->CreateBuffer(vertexBufferCreateInfo);

	Ck::Renderer::BufferCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.Usage = Ck::Renderer::BufferUsageFlagBits::Index;
	indexBufferCreateInfo.Size = indices->GetIndexCount() * ToDataType(indices->GetIndexType()).GetSize();
	Ck::Ref<Ck::Renderer::Buffer> indexBuffer = renderDevice->CreateBuffer(indexBufferCreateInfo);

	Ck::Renderer::ShaderProgramCreateInfo shaderProgramCreateInfo;
	shaderProgramCreateInfo.Type = Ck::Renderer::ShaderProgramType::Graphic;
	shaderProgramCreateInfo.ShaderCount = 2;
	shaderProgramCreateInfo.Shaders[0] = LoadShaderFromPath(*renderDevice, "builtin://hello_triangle/resources/shaders/default.vert.spv", Ck::Renderer::ShaderType::Vertex);
	shaderProgramCreateInfo.Shaders[1] = LoadShaderFromPath(*renderDevice, "builtin://hello_triangle/resources/shaders/default.frag.spv", Ck::Renderer::ShaderType::Fragment);
	Ck::Ref<Ck::Renderer::ShaderProgram> shaderProgram = renderDevice->CreateShaderProgram(shaderProgramCreateInfo);
	Ck::MaterialProgramInterface interface;
	Ck::Ref<Ck::MaterialProgram> materialProgram = Ck::MaterialProgram::New("default", interface, shaderProgram);

	Ck::Renderer::RenderSurfaceCreateInfo renderSurfaceCreateInfo;
	renderSurfaceCreateInfo.Window = window;
	renderSurfaceCreateInfo.DepthStencilFormat = Ck::PixelFormat::DepthStencil(24, 8);
	renderSurfaceCreateInfo.Samples = Ck::Renderer::RasterizationSamples::e4;
	Ck::Ref<Ck::Renderer::RenderSurface> renderSurface = renderDevice->CreateRenderSurface(renderSurfaceCreateInfo);

	Ck::Renderer::RenderContextCreateInfo renderContextCreateInfo;
	renderContextCreateInfo.ConcurrentFrameCount = renderSurface->GetBufferCount();
	Ck::Ref<Ck::Renderer::RenderContext> renderContext = renderDevice->CreateRenderContext(renderContextCreateInfo);

	Ck::Ref<Ck::RenderQueue> renderQueue = Ck::RenderQueue::New(nullptr, Ck::Material::ShadingMode::Phong);
	while (window->PollEvents())
	{
		Ck::Renderer::FrameContext* frameContext = renderContext->BeginFrame();

		// Transfer
		{
			Ck::Ref<Ck::Renderer::CommandList> commandList = frameContext->CreateCommandList({ Ck::Renderer::CommandListUsage::Transfer });

			commandList->Begin(nullptr);
			{
				Ck::Renderer::BufferUploadInfo uploadInfo{ 0, vertexBuffer->GetSize(), vertices->GetPointer() };
				commandList->UploadBuffer(vertexBuffer.Get(), 1, &uploadInfo);
			}
			{
				Ck::Renderer::BufferUploadInfo uploadInfo{ 0, indexBuffer->GetSize(), indices->GetPointer() };
				commandList->UploadBuffer(indexBuffer.Get(), 1, &uploadInfo);
			}
			commandList->End();

			renderContext->ExecuteCommandLists(Ck::Renderer::CommandQueueType::Transfer, 1, &commandList, nullptr);
			renderContext->SignalQueue(Ck::Renderer::CommandQueueType::Transfer);
			renderContext->WaitQueue(Ck::Renderer::CommandQueueType::Graphic, Ck::Renderer::CommandQueueType::Transfer);
		}

		// Render
		{
			Ck::StaticMeshRecordInfo recordInfo;
			recordInfo.Count = 3;
			recordInfo.VertexBufferCount = 1;
			recordInfo.VertexBuffers[0].Buffer = vertexBuffer.Get();
			recordInfo.VertexBuffers[0].VertexLayout = vertexLayout.Get();
			recordInfo.IndexBuffer = indexBuffer.Get();
			recordInfo.IndexType = indices->GetIndexType();
			//recordInfo.Program = materialProgram.Get();
			renderQueue->PushStaticMesh(recordInfo);
		}

		// Draw
		{
			Ck::Renderer::Framebuffer* framebuffer = frameContext->AcquireNextFramebuffer(renderSurface.Get());
			if (framebuffer)
			{
				Ck::Ref<Ck::Renderer::CommandList> commandList = frameContext->CreateCommandList({ Ck::Renderer::CommandListUsage::Graphic });

				commandList->Begin(nullptr);
				commandList->BeginRenderPass({ Ck::Renderer::RenderPassMode::Clear, framebuffer });

				Ck::RecordDrawContext drawContext(*frameContext);
				renderQueue->Flush(*commandList, drawContext);

				commandList->EndRenderPass();
				commandList->End();

				renderContext->ExecuteCommandLists(Ck::Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);
			}
		}
		renderQueue->Clear();

		renderContext->EndFrame();
	}

	return Ck::Main::ExitCode::Success;
}
