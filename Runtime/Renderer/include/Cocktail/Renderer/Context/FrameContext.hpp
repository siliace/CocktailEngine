#ifndef COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Renderer/Buffer/BufferUsage.hpp>
#include <Cocktail/Renderer/Command/Allocator/CommandListAllocator.hpp>
#include <Cocktail/Renderer/Memory/MemoryType.hpp>

namespace Ck::Renderer
{
	class BufferAllocator;
	class RenderSurface;

	using FrameToken = Uint64;

	/**
	 * \brief Interface exposing creation of GPU resources to generate a single frame
	 */
	class FrameContext : public Implements<FrameContext, CommandListAllocator>
	{
	public:

		/**
		 * \brief 
		 * \param renderSurface 
		 * \return 
		 */
		virtual Framebuffer* AcquireNextFramebuffer(RenderSurface* renderSurface) = 0;

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
