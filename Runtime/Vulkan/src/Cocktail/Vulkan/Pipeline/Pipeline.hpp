#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINE_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Pipeline : public Inherit<Pipeline, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit Pipeline(const Ref<RenderDevice>& renderDevice);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Ref<PipelineLayout> GetLayout() const = 0;
		
		/**
		 * \brief 
		 * \return 
		 */
		virtual VkPipeline GetHandle() const = 0;

	protected:

		Ref<RenderDevice> mRenderDevice;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINE_HPP
