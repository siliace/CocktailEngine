#ifndef COCKTAIL_VULKAN_SHADER_VALIDATIONCACHE_HPP
#define COCKTAIL_VULKAN_SHADER_VALIDATIONCACHE_HPP

#include <volk.h>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief
	 */
	struct ValidationCacheCreateInfo
	{
		/**
		 * \brief
		 */
		std::size_t InitialDataSize = 0;

		/**
		 * \brief
		 */
		const void* InitialData = nullptr;

		/**
		 * \brief Specifies the name of the PipelineCache to create
		 * If the extension Debug is not supported by the RenderDevice creating the ValidationCache, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};

	class ValidationCache
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		ValidationCache(Ref<RenderDevice> renderDevice, const ValidationCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~ValidationCache();

		/**
		 * \brief 
		 * \param other 
		 */
		void Merge(const ValidationCache& other) const;

		/**
		 * \brief
		 * \param name
		 */
		void SetObjectName(const char* name) const;

		/**
		 * \brief
		 * \return
		 */
		Ref<RenderDevice> GetRenderDevice() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<unsigned char> GetCacheData() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkValidationCacheEXT GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkValidationCacheEXT mHandle;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_VALIDATIONCACHE_HPP
