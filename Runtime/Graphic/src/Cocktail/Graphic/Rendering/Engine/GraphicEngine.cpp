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
	namespace
	{
		class RenderDeviceFactory
		{
		public:

			static std::shared_ptr<Renderer::RenderDevice> CreateVulkanRenderDevice(bool enableValidation)
			{
				Vulkan::RenderDeviceCreateInfo createInfo;
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

		Connect(mRenderDevice->OnDebugMessage(), [](LogLevel logLevel, Renderer::MessageType messageType, std::string_view message) {
			Log::Trace(logLevel, "Debug message of type {} from RenderDevice : {}", Enum<Renderer::MessageType>::ToString(messageType), message);
		});

		mRenderContext = mRenderDevice->CreateRenderContext({ 3, "" });

		mResourceUploader = std::make_shared<ResourceUploader>();
		mMaterialProgramManager = std::make_shared<MaterialProgramManager>(mRenderDevice);

		for (SamplerType samplerType : Enum<SamplerType>::Values)
		{
			Renderer::SamplerCreateInfo samplerCreateInfo;
			samplerCreateInfo.EnableAnisotropy = true;
			samplerCreateInfo.MaxAnisotropy = 8.f;
			samplerCreateInfo.Name = Enum<SamplerType>::ToString(samplerType).data();

			switch (samplerType)
			{
			case SamplerType::TrilinearClamp:
			case SamplerType::TrilinearWrap:
				samplerCreateInfo.MipmapMode = Renderer::SamplerMipmapMode::Linear;
				break;

			default:
				samplerCreateInfo.MipmapMode = Renderer::SamplerMipmapMode::Nearest;
				break;
			}

			switch (samplerType)
			{
			case SamplerType::LinearClamp:
			case SamplerType::LinearWrap:
			case SamplerType::TrilinearClamp:
			case SamplerType::TrilinearWrap:
				samplerCreateInfo.MinFilter = Renderer::Filter::Linear;
				samplerCreateInfo.MagFilter = Renderer::Filter::Linear;
				break;

			default:
				samplerCreateInfo.MinFilter = Renderer::Filter::Nearest;
				samplerCreateInfo.MagFilter = Renderer::Filter::Nearest;
				break;
			}
	
			switch (samplerType)
			{
			case SamplerType::LinearWrap:
			case SamplerType::NearestWrap:
			case SamplerType::TrilinearWrap:
				samplerCreateInfo.AddressModeU = Renderer::SamplerAddressMode::Repeat;
				samplerCreateInfo.AddressModeV = Renderer::SamplerAddressMode::Repeat;
				samplerCreateInfo.AddressModeW = Renderer::SamplerAddressMode::Repeat;
				break;

			case SamplerType::LinearClamp:
			case SamplerType::NearestClamp:
			case SamplerType::TrilinearClamp:
				samplerCreateInfo.AddressModeU = Renderer::SamplerAddressMode::ClampToEdge;
				samplerCreateInfo.AddressModeV = Renderer::SamplerAddressMode::ClampToEdge;
				samplerCreateInfo.AddressModeW = Renderer::SamplerAddressMode::ClampToEdge;
				break;
			}

			mSamplers[samplerType] = mRenderDevice->CreateSampler(samplerCreateInfo);
		}
	}

    GraphicEngine::~GraphicEngine()
    {
		mRenderContext->Flush();
		mRenderContext->Synchronize();
    }

    std::shared_ptr<VertexBuffer> GraphicEngine::CreateVertexBuffer(std::shared_ptr<VertexArray> vertices, bool updatable, std::string_view name)
    {
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(shared_from_this(), std::move(vertices), name);
		vertexBuffer->Upload();

		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> GraphicEngine::CreateIndexBuffer(std::shared_ptr<IndexArray> indices, bool updatable, std::string_view name)
	{
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(shared_from_this(), std::move(indices), name);
		indexBuffer->Upload();

		return indexBuffer;
	}

	std::shared_ptr<TextureResource> GraphicEngine::CreateTextureSampler(std::shared_ptr<MipMaps> mipMaps, std::string_view name)
	{
		std::shared_ptr<MipMapsTextureResource> textureResource = std::make_shared<MipMapsTextureResource>(shared_from_this(), mipMaps, name);
		for (unsigned int layer = 0; layer < mipMaps->GetArrayLayerCount(); layer++)
			textureResource->LoadLevels(layer, 0, mipMaps->GetMipMapCount() - 1);

		if (!mipMaps->IsComplete())
			mGeneratingMipMaps.push_back(textureResource);

		return textureResource;
	}

	void GraphicEngine::UploadBuffer(std::shared_ptr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data)
	{
		mResourceUploader->RequestBufferUpload(buffer, offset, length, data);
	}

	void GraphicEngine::UploadTexture(std::shared_ptr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data)
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

		if (!mGeneratingMipMaps.empty())
		{
			Renderer::CommandList* commandList = mRenderContext->CreateCommandList({ Renderer::CommandListUsageBits::Graphic });

			commandList->Begin(nullptr);
			for (std::shared_ptr<TextureResource> generatingMipMap : mGeneratingMipMaps)
				generatingMipMap->GenerateMipMaps(*commandList);
			commandList->End();

			mRenderContext->SubmitCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);

			mGeneratingMipMaps.clear();
		}
	}

	void GraphicEngine::Present()
	{
		mRenderContext->Flush();
		FlushTransfer();
	}

	std::shared_ptr<Renderer::RenderDevice> GraphicEngine::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	std::shared_ptr<Renderer::RenderContext> GraphicEngine::GetRenderContext() const
	{
		return mRenderContext;
	}

	std::shared_ptr<MaterialProgramManager> GraphicEngine::GetMaterialProgramManager() const
	{
		return mMaterialProgramManager;
	}

	std::shared_ptr<Renderer::Sampler> GraphicEngine::GetSampler(SamplerType samplerType) const
	{
		return mSamplers[samplerType];
	}
}
