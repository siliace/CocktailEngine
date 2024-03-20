#ifndef COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Renderer/Command/Allocator/CommandListAllocator.hpp>

namespace Ck::Renderer
{
	using FrameToken = Uint64;

	struct BufferArea
	{
		Buffer* Buffer = nullptr;
		std::size_t BaseOffset = 0;
		std::size_t Range = 0;
	};

	class BufferAllocator : public Implements<BufferAllocator, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param size
		 * \param data 
		 * \return 
		 */
		virtual BufferArea PushData(std::size_t size, const void* data) = 0;

		/**
		 * \brief 
		 * \param size 
		 */
		virtual void Reserve(std::size_t size) = 0;
	};

	/**
	 * \brief Interface exposing creation of GPU resources to generate a single frame
	 */
	class FrameContext : public Implements<FrameContext, CommandListAllocator>
	{
	public:

		/**
		 * \brief 
		 * \param usage 
		 * \param memoryType 
		 * \return 
		 */
		virtual BufferAllocator* GetBufferAllocator(BufferUsageFlags usage, MemoryType memoryType) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual FrameToken GetToken() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP
