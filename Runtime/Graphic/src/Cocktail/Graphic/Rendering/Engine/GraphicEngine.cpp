#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/MipMapsTextureResource.hpp>

#include <Cocktail/Renderer/Context/RenderContext.hpp>
#include <Cocktail/Renderer/Context/RenderContextCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/SamplerCreateInfo.hpp>

#include <Cocktail/Vulkan/Vulkan.hpp>

#ifdef NDEBUG
static constexpr bool GraphicEngine_EnableDebug = false;
#else
static constexpr bool GraphicEngine_EnableDebug = true;
#endif

namespace Ck
{
	COCKTAIL_DEFINE_LOG_CATEGORY(GraphicEngineLogCategory);

	namespace
	{
		class RenderDeviceFactory
		{
		public:

			static UniquePtr<Renderer::RenderDevice> CreateVulkanRenderDevice(bool enableValidation)
			{
				Vulkan::RenderDeviceCreateInfo createInfo;
				createInfo.ApplicationName = "CocktailEngine";
				createInfo.ApiVersion = Vulkan::GetSupportedApiVersion();
				createInfo.EnableValidation = enableValidation;
				return Vulkan::CreateRenderDevice(createInfo);
			}
		};
	}

	GraphicEngine::GraphicEngine(Renderer::GraphicApi api) 
	{
		switch (api)
		{
		case Renderer::GraphicApi::Vulkan:
			mRenderDevice = RenderDeviceFactory::CreateVulkanRenderDevice(GraphicEngine_EnableDebug);
			break;

		default:
			assert(false);
		}

		Connect(mRenderDevice->OnDebugMessage(), [](LogLevel logLevel, Renderer::MessageType messageType, AsciiStringView message) {
			CK_LOG(GraphicEngineLogCategory, logLevel, CK_TEXT("Debug message of type %hs from RenderDevice : %hs"), messageType, message);
		});

		mRenderContext = mRenderDevice->CreateRenderContext({ 3, "" });

		mResourceUploader = MakeShared<ResourceUploader>();
		mMaterialProgramManager = MakeShared<MaterialProgramManager>(mRenderDevice.Get());
	}

    GraphicEngine::~GraphicEngine()
    {
		mRenderContext->Flush();
		mRenderContext->Synchronize();
    }

    SharedPtr<VertexBuffer> GraphicEngine::CreateVertexBuffer(SharedPtr<VertexArray> vertices, bool updatable, const AnsiChar* name)
    {
		SharedPtr<VertexBuffer> vertexBuffer = MakeShared<VertexBuffer>(AsShared(), std::move(vertices), name);
		vertexBuffer->Upload();

		return vertexBuffer;
	}

	SharedPtr<IndexBuffer> GraphicEngine::CreateIndexBuffer(SharedPtr<IndexArray> indices, bool updatable, const AnsiChar* name)
	{
		SharedPtr<IndexBuffer> indexBuffer = MakeShared<IndexBuffer>(AsShared(), std::move(indices), name);
		indexBuffer->Upload();

		return indexBuffer;
	}

    SharedPtr<StorageBuffer> GraphicEngine::CreateStorageBuffer(std::size_t size, bool updatable, const AnsiChar* name)
    {
	    return MakeShared<StorageBuffer>(AsShared(), size, name);
    }

    SharedPtr<TextureResource> GraphicEngine::CreateTextureSampler(SharedPtr<MipMaps> mipMaps, const AnsiChar* name)
	{
		SharedPtr<MipMapsTextureResource> textureResource = MakeShared<MipMapsTextureResource>(AsShared(), mipMaps, name);
		for (unsigned int layer = 0; layer < mipMaps->GetArrayLayerCount(); layer++)
			textureResource->LoadLevels(layer, 0, mipMaps->GetMipMapCount() - 1);

		if (!mipMaps->IsComplete())
			mGeneratingMipMaps.Add(textureResource);

		return textureResource;
	}

	void GraphicEngine::UploadBuffer(SharedPtr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data)
	{
		mResourceUploader->RequestBufferUpload(buffer, offset, length, data);
	}

	void GraphicEngine::UploadTexture(SharedPtr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data)
	{
		mResourceUploader->RequestTextureUpload(texture, arrayLayer, mipMapLevel, data);
	}

	void GraphicEngine::FlushTransfer()
	{
		if (mResourceUploader->HasPendingTransfer())
		{
			Renderer::CommandList* commandList = mRenderContext->CreateCommandList({ Renderer::CommandListUsageBits::Transfer });

			commandList->Begin(nullptr);
			mResourceUploader->Flush(*commandList);
			commandList->End();

			mRenderContext->SubmitCommandLists(Renderer::CommandQueueType::Transfer, 1, &commandList, nullptr);
			mRenderContext->SignalQueue(Renderer::CommandQueueType::Transfer);
			mRenderContext->WaitQueue(Renderer::CommandQueueType::Graphic, Renderer::CommandQueueType::Transfer);
		}

		if (!mGeneratingMipMaps.IsEmpty())
		{
			Renderer::CommandList* commandList = mRenderContext->CreateCommandList({ Renderer::CommandListUsageBits::Graphic });

			commandList->Begin(nullptr);
			for (SharedPtr<TextureResource> generatingMipMap : mGeneratingMipMaps)
				generatingMipMap->GenerateMipMaps(*commandList);
			commandList->End();

			mRenderContext->SubmitCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);

			mGeneratingMipMaps.Clear();
		}
	}

	void GraphicEngine::Present()
	{
		mRenderContext->Flush();
		FlushTransfer();
	}

	Renderer::RenderDevice* GraphicEngine::GetRenderDevice() const
	{
		return mRenderDevice.Get();
	}

	SharedPtr<Renderer::RenderContext> GraphicEngine::GetRenderContext() const
	{
		return mRenderContext;
	}

	SharedPtr<MaterialProgramManager> GraphicEngine::GetMaterialProgramManager() const
	{
		return mMaterialProgramManager;
	}
}
