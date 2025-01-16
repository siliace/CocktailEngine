#ifndef COCKTAIL_VULKAN_BUFFER_BUFFER_HPP
#define COCKTAIL_VULKAN_BUFFER_BUFFER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>
#include <Cocktail/Renderer/Buffer/BufferCreateInfo.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Buffer : public Inherit<Buffer, Object, Renderer::Buffer>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Buffer(Ref<RenderDevice> renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

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

		Ref<RenderDevice> mRenderDevice;
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
