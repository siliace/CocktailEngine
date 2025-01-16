#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERALLOCATOR_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERALLOCATOR_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Renderer/Buffer/BufferArea.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
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
}

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERALLOCATOR_HPP
