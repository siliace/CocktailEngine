#ifndef COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API ResourceUploader : public Extends<ResourceUploader, Object>
	{
	public:

		void RequestBufferUpload(Ref<BufferResource> buffer, std::size_t offset, std::size_t length, const void* data);

		void RequestTextureUpload(Ref<TextureResource> texture, unsigned int arrayLayer, unsigned int mipMapLevel, const void* data);

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
			Ref<BufferResource> Buffer;
			std::vector<Renderer::BufferUploadInfo> UploadInfo;
		};

		struct TextureUploadRequest
		{
			Ref<TextureResource> Texture;
			std::vector<Renderer::TextureUploadInfo> UploadInfo;
		};

		std::vector<BufferUploadRequest> mBufferUploadRequests;
		std::vector<TextureUploadRequest> mTextureUploadRequests;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_ENGIN_RESOURCEUPLOADER_HPP
