#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERVIEW_HPP

#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	class Buffer;

	/**
     * \brief 
     */
	class BufferView : public Implements<BufferView, RenderDeviceObject>
	{
	public:

		/**
	     * \brief 
	     * \return 
	     */
		virtual Ref<Buffer> GetBuffer() const = 0;

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
