#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	class Buffer;

	/**
     * \brief Interface representing an opaque handle to a GPU buffer view object
     * A BufferView is a contiguous range of a buffer and a specific format to be used to interpret the data
     */
	class BufferView : public RenderDeviceObject
	{
	public:

		/**
	     * \brief Get the underling Buffer
	     * \return The underlying Buffer
	     */
		virtual std::shared_ptr<Buffer> GetBuffer() const = 0;

		/**
		 * \brief Get the format used to interpret Buffer data
		 * \return The interpretation format
		 */
		virtual PixelFormat GetFormat() const = 0;

		/**
		 * \brief The first byte of the Buffer, the BufferView can access
		 * \return The offset
		 */
		virtual std::size_t GetOffset() const = 0;

		/**
		 * \brief Get the number of byte of the buffer, the BufferView can access
		 * \return The range
		 */
		virtual std::size_t GetRange() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP
