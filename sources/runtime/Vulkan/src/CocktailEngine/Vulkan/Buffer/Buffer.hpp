#ifndef COCKTAILENGINE_VULKAN_BUFFER_BUFFER_HPP
#define COCKTAILENGINE_VULKAN_BUFFER_BUFFER_HPP

#include <CocktailEngine/Core/Memory/SharedFromThis.hpp>

#include <CocktailEngine/Renderer/Buffer/Buffer.hpp>
#include <CocktailEngine/Renderer/Buffer/BufferCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class DeviceMemoryBlock;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Buffer : public Renderer::Buffer, public SharedFromThis<Buffer>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Buffer(RenderDevice* renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Buffer() override;

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
		 * \brief 
		 * \return 
		 */
		bool IsExclusive() const override;

	    Renderer::MemoryPriority GetPriority() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::MemoryType GetMemoryType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ResourceType GetResourceType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::BufferUsageFlags GetUsage() const override;

		/**
		 * \brief 
		 * \param offset 
		 * \param length 
		 * \return 
		 */
		void* Map(std::size_t offset, std::size_t length);

		/**
		 * \brief 
		 */
		void Unmap();

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkBuffer GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkBuffer mHandle;
		Renderer::BufferUsageFlags mUsage;
		Renderer::MemoryType mMemoryType;
	    Renderer::MemoryPriority mMemoryPriority;
		std::size_t mSize;
		bool mExclusive;
		DeviceMemoryBlock* mMemoryBlock;
	};
}

#endif // COCKTAILENGINE_VULKAN_BUFFER_BUFFER_HPP
