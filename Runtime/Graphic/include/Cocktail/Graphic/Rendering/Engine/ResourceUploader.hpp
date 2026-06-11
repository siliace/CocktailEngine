#ifndef COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP

#include <Cocktail/Core/Array.hpp>

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API ResourceUploader
	{
	public:

		void RequestBufferUpload(SharedPtr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data);

		void RequestTextureUpload(SharedPtr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data);

		/**
		 * \brief 
		 * \return 
		 */
		bool HasPendingTransfer() const;

		/**
		 * \brief 
		 * \param commandList 
		 */
		void Flush(Renderer::CommandList& commandList);

	private:

		struct BufferUploadRequest
		{
			SharedPtr<BufferResource> Buffer;
			Array<Renderer::BufferUploadInfo> UploadInfo;
		};

		struct TextureUploadRequest
		{
			SharedPtr<TextureResource> Texture;
			Array<Renderer::TextureUploadInfo> UploadInfo;
		};

		Array<BufferUploadRequest> mBufferUploadRequests;
		Array<TextureUploadRequest> mTextureUploadRequests;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP
