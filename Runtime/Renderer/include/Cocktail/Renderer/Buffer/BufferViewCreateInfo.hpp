#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERVIEWCREATEINFO_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERVIEWCREATEINFO_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/Memory/Ref.hpp>

namespace Ck::Renderer
{
    class Buffer;

    /**
     * \brief BufferView descriptor structure
     * Describe the set of parameters to create a BufferView
     * The addition of \p Range and \p Offset should not excide the size of \p Buffer
     * \see Buffer::GetSize
     * \see RenderDevice::CreateBufferView
     */
    struct BufferViewCreateInfo
    {
        /**
         * \brief Specifies the Buffer on which create the BufferView
         */
        Ref<Buffer> Buffer;

        /**
         * \brief Specifies the Format describing the layout of the data elements in the \p Buffer
         */
        PixelFormat Format;

        /**
         * \brief Specifies the offset (in bytes) from the base address of the \p Buffer
         * Accesses to the created BufferView from shaders will be done relatively to this offset
         */
        std::size_t Offset = 0;
        
        /**
         * \brief Specifies the size (in bytes) of the BufferView to create
         */
        std::size_t Range = 0;

        /**
         * \brief Specifies the name of the BufferView to create
         * If the extension Debug is not supported by the RenderDevice creating the BufferView, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERVIEWCREATEINFO_HPP