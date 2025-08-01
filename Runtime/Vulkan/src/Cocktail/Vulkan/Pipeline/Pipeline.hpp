#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINE_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINE_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Vulkan
{
	class PipelineLayout;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Pipeline : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit Pipeline(std::shared_ptr<RenderDevice> renderDevice);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::shared_ptr<PipelineLayout> GetLayout() const = 0;
		
		/**
		 * \brief 
		 * \return 
		 */
		virtual VkPipeline GetHandle() const = 0;

	protected:

		std::shared_ptr<RenderDevice> mRenderDevice;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINE_HPP
