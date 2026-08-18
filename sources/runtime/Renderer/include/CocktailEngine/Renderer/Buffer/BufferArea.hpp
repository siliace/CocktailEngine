#ifndef COCKTAILENGINE_RENDERER_BUFFER_BUFFERAREA_HPP
#define COCKTAILENGINE_RENDERER_BUFFER_BUFFERAREA_HPP

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
		const Buffer* BufferResource = nullptr;

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

#endif // COCKTAILENGINE_RENDERER_BUFFER_BUFFERAREA_HPP
