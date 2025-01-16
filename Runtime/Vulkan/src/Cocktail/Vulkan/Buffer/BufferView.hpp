#ifndef COCKTAIL_VULKAN_BUFFER_BUFFERVIEW_HPP
#define COCKTAIL_VULKAN_BUFFER_BUFFERVIEW_HPP

#include <Cocktail/Renderer/Buffer/BufferView.hpp>
#include <Cocktail/Renderer/Buffer/BufferViewCreateInfo.hpp>

#include <Cocktail/Vulkan/Buffer/Buffer.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class BufferView : public Inherit<BufferView, Object, Renderer::BufferView>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		BufferView(Ref<RenderDevice> renderDevice, const Renderer::BufferViewCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~BufferView() override;

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
		Ref<Renderer::Buffer> GetBuffer() const override;

		/**
		 * \brief 
		 * \return 
		 */
		PixelFormat GetFormat() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetOffset() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetRange() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkBufferView GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		Ref<Buffer> mBuffer;
		PixelFormat mFormat;
		std::size_t mOffset;
		std::size_t mRange;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkBufferView mHandle;
	};
}

#endif // COCKTAIL_VULKAN_BUFFER_BUFFERVIEW_HPP
