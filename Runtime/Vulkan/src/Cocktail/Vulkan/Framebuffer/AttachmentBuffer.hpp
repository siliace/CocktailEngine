#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_ATTACHMENTBUFFER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_ATTACHMENTBUFFER_HPP

#include <Cocktail/Core/Extent3D.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>

namespace Ck::Vulkan
{
	class RenderPass;
	class TextureView;

	/**
	 * \brief Utility class containing a multisample attachment of a framebuffer and its resolve counterpart
	 */
	class AttachmentBuffer
	{
	public:

		/**
		 * \brief Constructor
		 * \param renderDevice
		 * \param resolveAttachment
		 * \param samples
		 */
		AttachmentBuffer(RenderDevice& renderDevice, std::shared_ptr<TextureView> resolveAttachment, Renderer::RasterizationSamples samples);

		/**
		 * \brief
		 * \return
		 */
		bool IsMultisample() const;

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<TextureView> GetMultisampleAttachment() const;

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<TextureView> GetResolveAttachment() const;

		/**
		 * \brief
		 * \return
		 */
		Extent3D<unsigned int> GetSize() const;

	private:

		std::shared_ptr<TextureView> mMultisampleAttachment;
		std::shared_ptr<TextureView> mResolveAttachments;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_ATTACHMENTBUFFER_HPP
