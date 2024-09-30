#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

namespace Ck
{
	class MipMapsSampler : public Extends<MipMapsSampler, TextureResource>
	{
	public:

		MipMapsSampler(Ref<GraphicEngine> graphicEngine, Ref<Renderer::Texture> texture, Ref<MipMaps> mipMaps) :
			mGraphicEngine(std::move(graphicEngine)),
			mTexture(std::move(texture)),
			mMipMaps(std::move(mipMaps))
		{
			Renderer::TextureViewCreateInfo viewCreateInfo;
			viewCreateInfo.Texture = mTexture;
			viewCreateInfo.Type = Renderer::TextureViewType::e2D;

			mView = mGraphicEngine->GetRenderDevice()->CreateTextureView(viewCreateInfo);
		}

		/**
		 * \brief 
		 * \param arrayLayer 
		 * \param mipMapLevel 
		 */
		void LoadLevel(unsigned int arrayLayer, unsigned int mipMapLevel)
		{
			mGraphicEngine->UploadTexture(Self(), arrayLayer, mipMapLevel, mMipMaps->GetLevel(arrayLayer, mipMapLevel).GetPixels());
		}

		void GenerateMipMaps(Renderer::CommandList& commandList) const override
		{
			commandList.GenerateMipMaps(mTexture.Get(), Renderer::ResourceState::GraphicShaderResource, Renderer::TextureSubResource::AllLayersAllLevels(*mTexture, 0, 1));
		}

		Ref<Renderer::TextureView> GetView() const override
		{
			return mView;
		}

		Ref<GraphicEngine> GetGraphicEngine() const override
		{
			return mGraphicEngine;
		}

		Renderer::Texture* GetUnderlyingResource() const override
		{
			return mTexture.Get();
		}

	private:

		Ref<GraphicEngine> mGraphicEngine;
		Ref<Renderer::Texture> mTexture;
		Ref<MipMaps> mMipMaps;
		Ref<Renderer::TextureView> mView;
	};

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

	Ref<VertexBuffer> GraphicEngine::CreateVertexBuffer(Ref<VertexArray> vertices, bool updatable, std::string_view name)
	{
		Renderer::BufferCreateInfo bufferCreateInfo;
		bufferCreateInfo.Usage = Renderer::BufferUsageFlagBits::Vertex;
		bufferCreateInfo.Size = vertices->GetSize();
		bufferCreateInfo.MemoryType = !updatable ? Renderer::MemoryType::Static : Renderer::MemoryType::Streamed;
		bufferCreateInfo.Name = name.data();
		Ref<Renderer::Buffer> buffer = mRenderDevice->CreateBuffer(bufferCreateInfo);

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::New(Self(), std::move(buffer), std::move(vertices));
		vertexBuffer->Upload();

		return vertexBuffer;
	}

	Ref<IndexBuffer> GraphicEngine::CreateIndexBuffer(Ref<IndexArray> indices, bool updatable, std::string_view name)
	{
		Renderer::BufferCreateInfo bufferCreateInfo;
		bufferCreateInfo.Usage = Renderer::BufferUsageFlagBits::Index;
		bufferCreateInfo.Size = indices->GetSize();
		bufferCreateInfo.MemoryType = !updatable ? Renderer::MemoryType::Static : Renderer::MemoryType::Streamed;
		bufferCreateInfo.Name = name.data();
		Ref<Renderer::Buffer> buffer = mRenderDevice->CreateBuffer(bufferCreateInfo);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::New(Self(), std::move(buffer), std::move(indices));
		indexBuffer->Upload();

		return indexBuffer;
	}

	Ref<TextureResource> GraphicEngine::CreateTextureSampler(Ref<MipMaps> mipMaps)
	{
		Renderer::TextureCreateInfo textureCreateInfo;
		textureCreateInfo.Type = Renderer::TextureType::e2D;
		textureCreateInfo.Format = mipMaps->GetPixelFormat();
		textureCreateInfo.Size = mipMaps->GetBaseSize();
		textureCreateInfo.MipMapsCount = mipMaps->GetMipMapCount();
		textureCreateInfo.ArrayLayerCount = mipMaps->GetArrayLayerCount();
		Ref<Renderer::Texture> texture = mRenderDevice->CreateTexture(textureCreateInfo);

		Ref<MipMapsSampler> sampler = MipMapsSampler::New(Self(), std::move(texture), mipMaps);
		for (unsigned int arrayLayer = 0; arrayLayer < mipMaps->GetArrayLayerCount(); arrayLayer++)
		{
			for (unsigned int mipMapLevel = 0; mipMapLevel < mipMaps->GetMipMapCount(); mipMapLevel++)
			{
				sampler->LoadLevel(arrayLayer, mipMapLevel);
			}
		}

		mGeneratingMipMaps.push_back(sampler);

		return sampler;
	}

	void GraphicEngine::UploadBuffer(Ref<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data)
	{
		mResourceUploader->RequestBufferUpload(buffer->GetUnderlyingResource(), offset, length, data);
	}

	void GraphicEngine::UploadTexture(Ref<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data)
	{
		mResourceUploader->RequestTextureUpload(texture->GetUnderlyingResource(), arrayLayer, mipMapLevel, data);
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
