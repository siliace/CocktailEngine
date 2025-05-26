#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	class Buffer;

	/**
     * \brief 
     */
	class BufferView : public RenderDeviceObject
	{
	public:

		/**
	     * \brief 
	     * \return 
	     */
		virtual std::shared_ptr<Buffer> GetBuffer() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual PixelFormat GetFormat() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::size_t GetOffset() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::size_t GetRange() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP
