#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCacheCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	using PipelineStateHash = Uint64;

	/**
	 * \brief 
	 */
	class PipelineCache : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		PipelineCache(RenderDevice* renderDevice, const PipelineCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~PipelineCache() override;

		/**
		 * \brief 
		 * \param other 
		 */
		void Merge(const PipelineCache& other) const;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		ByteArray GetCacheData() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkPipelineCache GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipelineCache mHandle;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
