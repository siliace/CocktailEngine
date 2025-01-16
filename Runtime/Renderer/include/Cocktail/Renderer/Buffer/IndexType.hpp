#ifndef COCKTAIL_RENDERER_BUFFER_INDEXTYPE_HPP
#define COCKTAIL_RENDERER_BUFFER_INDEXTYPE_HPP

#include <Cocktail/Core/DataType.hpp>

#include <Cocktail/Renderer/Export.hpp>

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

    /**
     * \brief Utility function to convert an IndexType to its equivalent DataType
     * \param indexType 
     * \return 
     */
    DataType COCKTAIL_RENDERER_API ToDataType(IndexType indexType);
}

#endif // COCKTAIL_RENDERER_BUFFER_INDEXTYPE_HPP