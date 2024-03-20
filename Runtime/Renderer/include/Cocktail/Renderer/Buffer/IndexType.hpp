#ifndef COCKTAIL_RENDERER_BUFFER_INDEXTYPE_HPP
#define COCKTAIL_RENDERER_BUFFER_INDEXTYPE_HPP

namespace Ck::Renderer
{
    /**
     * \brief Enumeration of possible index types of index buffers
     */
    enum class IndexType
    {
        /**
         * \brief 2 bytes index type
         */
        Short,

        /**
         * \brief 4 bytes index type
         */
        Integer,

        /**
         * \brief 1 byte index type
         * \remark Is the extension ByteIndexType is not supported by a RenderDevice, creation of a buffer using this index type will fail
         * \see RenderDevice::IsSupportedExtension
         * \see RenderDeviceExtension::ByteIndexType
         */
        Byte,
    };
}

#endif // COCKTAIL_RENDERER_BUFFER_INDEXTYPE_HPP