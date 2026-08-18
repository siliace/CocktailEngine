#ifndef COCKTAILENGINE_VULKAN_FRAMEBUFFER_ATTACHMENTBUFFER_HPP
#define COCKTAILENGINE_VULKAN_FRAMEBUFFER_ATTACHMENTBUFFER_HPP

#include <CocktailEngine/Core/Extent3D.hpp>

#include <CocktailEngine/Renderer/RasterizationSamples.hpp>

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
		AttachmentBuffer(RenderDevice& renderDevice, SharedPtr<TextureView> resolveAttachment, Renderer::RasterizationSamples samples);

		/**
		 * \brief
		 * \return
		 */
		bool IsMultisample() const;

		/**
		 * \brief
		 * \return
		 */
		SharedPtr<TextureView> GetMultisampleAttachment() const;

		/**
		 * \brief
		 * \return
		 */
		SharedPtr<TextureView> GetResolveAttachment() const;

		/**
		 * \brief
		 * \return
		 */
		Extent3D<unsigned int> GetSize() const;

	private:

		SharedPtr<TextureView> mMultisampleAttachment;
		SharedPtr<TextureView> mResolveAttachments;
	};
}

#endif // COCKTAILENGINE_VULKAN_FRAMEBUFFER_ATTACHMENTBUFFER_HPP
