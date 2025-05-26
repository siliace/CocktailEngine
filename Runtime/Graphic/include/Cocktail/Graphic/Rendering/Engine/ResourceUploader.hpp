#ifndef COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP

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

		void RequestBufferUpload(std::shared_ptr<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data);

		void RequestTextureUpload(std::shared_ptr<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data);

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
			std::shared_ptr<BufferResource> Buffer;
			std::vector<Renderer::BufferUploadInfo> UploadInfo;
		};

		struct TextureUploadRequest
		{
			std::shared_ptr<TextureResource> Texture;
			std::vector<Renderer::TextureUploadInfo> UploadInfo;
		};

		std::vector<BufferUploadRequest> mBufferUploadRequests;
		std::vector<TextureUploadRequest> mTextureUploadRequests;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP
