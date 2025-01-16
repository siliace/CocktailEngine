#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERAREA_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERAREA_HPP

#include <utility>

namespace Ck::Renderer
{
	class Buffer;

	/**
	 * \brief 
	 */
	struct BufferArea
	{
		/**
		 * \brief 
		 */
		Buffer* Buffer = nullptr;

		/**
		 * \brief 
		 */
		std::size_t BaseOffset = 0;

		/**
		 * \brief 
		 */
		std::size_t Range = 0;
	};
}

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERAREA_HPP
