#ifndef COCKTAIL_VULKAN_BUFFER_BUFFER_HPP
#define COCKTAIL_VULKAN_BUFFER_BUFFER_HPP

#include <memory>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>
#include <Cocktail/Renderer/Buffer/BufferCreateInfo.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class DeviceMemoryBlock;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Buffer : public Renderer::Buffer, public std::enable_shared_from_this<Buffer>
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
		std::size_t mSize;
		bool mExclusive;
		DeviceMemoryBlock* mMemoryBlock;
	};
}

#endif // COCKTAIL_VULKAN_BUFFER_BUFFER_HPP
