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
	class BufferView : public Renderer::BufferView
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		BufferView(std::shared_ptr<RenderDevice> renderDevice, const Renderer::BufferViewCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Renderer::Buffer> GetBuffer() const override;

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

		std::shared_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<Buffer> mBuffer;
		PixelFormat mFormat;
		std::size_t mOffset;
		std::size_t mRange;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkBufferView mHandle;
	};
}

#endif // COCKTAIL_VULKAN_BUFFER_BUFFERVIEW_HPP
