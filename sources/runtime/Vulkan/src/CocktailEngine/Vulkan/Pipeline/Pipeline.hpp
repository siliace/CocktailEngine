#ifndef COCKTAILENGINE_VULKAN_PIPELINE_PIPELINE_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_PIPELINE_HPP

#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

#include <CocktailEngine/Vulkan/Pipeline/PipelineCreationFeedback.hpp>

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
		explicit Pipeline(RenderDevice* renderDevice);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief Retrieves the creation feedback the driver reported for this Pipeline
		 *
		 * Every entry is invalid when the RenderDeviceFeature::PipelineCreationFeedback feature
		 * is unsupported, or when the driver declined to report anything.
		 *
		 * \return The creation feedback
		 */
		const PipelineCreationFeedback& GetCreationFeedback() const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual SharedPtr<PipelineLayout> GetLayout() const = 0;
		
		/**
		 * \brief 
		 * \return 
		 */
		virtual VkPipeline GetHandle() const = 0;

	protected:

		RenderDevice* mRenderDevice;
		PipelineCreationFeedback mCreationFeedback;
	};
}

#endif // COCKTAILENGINE_VULKAN_PIPELINE_PIPELINE_HPP
