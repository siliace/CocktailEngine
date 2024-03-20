#ifndef COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Renderer/Command/Allocator/CommandListAllocator.hpp>

namespace Ck::Renderer
{
	using FrameToken = Uint64;

	/**
	 * \brief Interface exposing creation of GPU resources to generate a single frame
	 */
	class FrameContext : public Implements<FrameContext, CommandListAllocator>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		virtual FrameToken GetToken() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_FRAMECONTEXT_HPP
