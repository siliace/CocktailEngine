#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/MipMapsTextureResource.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

namespace Ck
{
	GraphicEngine::GraphicEngine(Application& application, Renderer::GraphicApi api) :
		mFrameContext(nullptr)
	{
		mRenderDevice = application.Invoke([&](const Ref<Renderer::RendererService>& rendererService) {
			Ref<Renderer::RenderDevice> renderDevice = rendererService->CreateRenderDevice(api, true);

			Connect(renderDevice->OnDebugMessage(), [](LogLevel logLevel, Renderer::MessageType messageType, std::string_view message) {
				Log::Trace(logLevel, "Debug message of type {} from RenderDevice : {}", Enum<Renderer::MessageType>::ToString(messageType), message);
			});

			return renderDevice;
		});

		mRenderContext = mRenderDevice->CreateRenderContext({ 1, 3, "" });
		mFrameContext = mRenderContext->BeginFrame();

		mResourceUploader = ResourceUploader::New();
		mMaterialProgramManager = MaterialProgramManager::New(mRenderDevice);
	}

    GraphicEngine::~GraphicEngine()
    {
		mRenderContext->EndFrame();
    }

    Ref<VertexBuffer> GraphicEngine::CreateVertexBuffer(Ref<VertexArray> vertices, bool updatable, std::string_view name)
    {
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::New(Self(), std::move(vertices), name);
		vertexBuffer->Upload();

		return vertexBuffer;
	}

	Ref<IndexBuffer> GraphicEngine::CreateIndexBuffer(Ref<IndexArray> indices, bool updatable, std::string_view name)
	{
		Ref<IndexBuffer> indexBuffer = IndexBuffer::New(Self(), std::move(indices), name);
		indexBuffer->Upload();

		return indexBuffer;
	}

	Ref<TextureResource> GraphicEngine::CreateTextureSampler(Ref<MipMaps> mipMaps, std::string_view name)
	{
		Ref<MipMapsTextureResource> textureResource = MipMapsTextureResource::New(Self(), mipMaps, name);
		for (unsigned int layer = 0; layer < mipMaps->GetArrayLayerCount(); layer++)
			textureResource->LoadLevels(layer, 0, mipMaps->GetMipMapCount() - 1);

		if (!mipMaps->IsComplete())
			mGeneratingMipMaps.push_back(textureResource);

		return textureResource;
	}

	void GraphicEngine::UploadBuffer(Ref<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data)
	{
		mResourceUploader->RequestBufferUpload(buffer, offset, length, data);
	}

	void GraphicEngine::UploadTexture(Ref<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data)
	{
		mResourceUploader->RequestTextureUpload(texture, arrayLayer, mipMapLevel, data);
	}

	void GraphicEngine::FlushTransfer()
	{
		if (mResourceUploader->HasPendingTransfer())
		{
			Ref<Renderer::CommandList> commandList = mFrameContext->CreateCommandList({ Renderer::CommandListUsage::Transfer });

			commandList->Begin(nullptr);
			mResourceUploader->Flush(*commandList);
			commandList->End();

			mRenderContext->ExecuteCommandLists(Renderer::CommandQueueType::Transfer, 1, &commandList, nullptr);
			mRenderContext->SignalQueue(Renderer::CommandQueueType::Transfer);
			mRenderContext->WaitQueue(Renderer::CommandQueueType::Graphic, Renderer::CommandQueueType::Transfer);
		}

		if (!mGeneratingMipMaps.empty())
		{
			Ref<Renderer::CommandList> commandList = mFrameContext->CreateCommandList({ Renderer::CommandListUsage::Graphic });

			commandList->Begin(nullptr);
			for (Ref<TextureResource> generatingMipMap : mGeneratingMipMaps)
				generatingMipMap->GenerateMipMaps(*commandList);
			commandList->End();

			mRenderContext->ExecuteCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);

			mGeneratingMipMaps.clear();
		}
	}

	void GraphicEngine::Present()
	{
		mRenderContext->EndFrame();

		mFrameContext = mRenderContext->BeginFrame();
		FlushTransfer();
	}

	Ref<Renderer::RenderDevice> GraphicEngine::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Ref<Renderer::RenderContext> GraphicEngine::GetRenderContext() const
	{
		return mRenderContext;
	}

	Renderer::FrameContext* GraphicEngine::GetFrameContext() const
	{
		return mFrameContext;
	}

	Ref<MaterialProgramManager> GraphicEngine::GetMaterialProgramManager() const
	{
		return mMaterialProgramManager;
	}
}
