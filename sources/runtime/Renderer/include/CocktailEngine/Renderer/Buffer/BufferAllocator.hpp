#ifndef COCKTAILENGINE_RENDERER_BUFFER_BUFFERALLOCATOR_HPP
#define COCKTAILENGINE_RENDERER_BUFFER_BUFFERALLOCATOR_HPP

#include <CocktailEngine/Renderer/Buffer/BufferArea.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	class BufferAllocator
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~BufferAllocator() = default;

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
}

#endif // COCKTAILENGINE_RENDERER_BUFFER_BUFFERALLOCATOR_HPP
