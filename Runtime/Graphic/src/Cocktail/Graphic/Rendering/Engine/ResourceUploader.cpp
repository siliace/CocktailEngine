#include <Cocktail/Graphic/Rendering/Engine/ResourceUploader.hpp>

namespace Ck
{
	void ResourceUploader::RequestBufferUpload(std::shared_ptr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data)
	{
		for (auto& bufferUploadRequest : mBufferUploadRequests)
		{
			if (bufferUploadRequest.Buffer == buffer)
			{
				bufferUploadRequest.UploadInfo.Add({ offset, length, data });
				return;
			}
		}

		BufferUploadRequest bufferUploadRequest{ buffer };
		bufferUploadRequest.UploadInfo.Add({ offset, length, data });
		mBufferUploadRequests.Add(std::move(bufferUploadRequest));
	}

	void ResourceUploader::RequestTextureUpload(std::shared_ptr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data)
	{
		Extent3D<unsigned int> textureSize = texture->GetSize();
		Extent3D<unsigned int> uploadSize = MakeExtent(textureSize.Width, textureSize.Height, 1u);
		for (auto& textureUploadRequest : mTextureUploadRequests)
		{
			if (textureUploadRequest.Texture == texture)
			{
				textureUploadRequest.UploadInfo.Add({ arrayLayer, mipMapLevel, uploadSize, MakeExtent(0u, 0u, 0u), data });
				return;
			}
		}

		TextureUploadRequest textureUploadRequest{ texture };
		textureUploadRequest.UploadInfo.Add({ arrayLayer, mipMapLevel, uploadSize, MakeExtent(0u, 0u, 0u), data });
		mTextureUploadRequests.Add(std::move(textureUploadRequest));
	}

	bool ResourceUploader::HasPendingTransfer() const
	{
		return !mBufferUploadRequests.IsEmpty() || !mTextureUploadRequests.IsEmpty();
	}

	void ResourceUploader::Flush(Renderer::CommandList& commandList)
	{
		for (const auto& bufferUploadRequest : mBufferUploadRequests)
			commandList.UploadBuffer(bufferUploadRequest.Buffer->GetUnderlyingResource(), bufferUploadRequest.UploadInfo.GetSize(), bufferUploadRequest.UploadInfo.GetData());

		for (const auto& textureUploadRequest : mTextureUploadRequests)
		{
			{
				Renderer::GpuBarrier* barriers = COCKTAIL_STACK_ALLOC(Renderer::GpuBarrier, textureUploadRequest.UploadInfo.GetSize());
				for (std::size_t i = 0; i < textureUploadRequest.UploadInfo.GetSize(); i++)
				{
					Renderer::TextureSubResource subResource;
					subResource.BaseArrayLayer = textureUploadRequest.UploadInfo[i].ArrayLayer;
					subResource.BaseMipMapLevel = textureUploadRequest.UploadInfo[i].Level;
					subResource.ArrayLayerCount = 1;
					subResource.MipMapLevelCount = 1;

					barriers[i] = Renderer::GpuBarrier::Of(
						textureUploadRequest.Texture->GetUnderlyingResource(), Renderer::ResourceState::Undefined, Renderer::ResourceState::CopyDestination,
						subResource
					);
				}
				commandList.Barrier(textureUploadRequest.UploadInfo.GetSize(), barriers);
			}

			commandList.UploadTexture(textureUploadRequest.Texture->GetUnderlyingResource(), Renderer::ResourceState::CopyDestination, textureUploadRequest.UploadInfo.GetSize(), textureUploadRequest.UploadInfo.GetData());

			{
				Renderer::GpuBarrier* barriers = COCKTAIL_STACK_ALLOC(Renderer::GpuBarrier, mTextureUploadRequests.GetSize());
				for (std::size_t i = 0; i < textureUploadRequest.UploadInfo.GetSize(); i++)
				{
					Renderer::TextureSubResource subResource;
					subResource.BaseArrayLayer = textureUploadRequest.UploadInfo[i].ArrayLayer;
					subResource.BaseMipMapLevel = textureUploadRequest.UploadInfo[i].Level;
					subResource.ArrayLayerCount = 1;
					subResource.MipMapLevelCount = 1;

					barriers[i] = Renderer::GpuBarrier::Of(
						textureUploadRequest.Texture->GetUnderlyingResource(), Renderer::ResourceState::CopyDestination, Renderer::ResourceState::GraphicShaderResource,
						subResource
					);
				}
				commandList.Barrier(textureUploadRequest.UploadInfo.GetSize(), barriers);
			}
		}

		mBufferUploadRequests.Clear();
		mTextureUploadRequests.Clear();
	}
}
