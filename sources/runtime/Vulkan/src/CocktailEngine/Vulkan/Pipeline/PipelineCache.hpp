#ifndef COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHE_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHE_HPP

#include <CocktailEngine/Core/Utility/ByteArray.hpp>

#include <CocktailEngine/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <CocktailEngine/Vulkan/Pipeline/PipelineCacheCreateInfo.hpp>

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
		LargeByteArray GetCacheData() const;

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

#endif // COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHE_HPP
