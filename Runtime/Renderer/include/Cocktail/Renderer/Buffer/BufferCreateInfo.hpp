#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERCREATEINFO_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERCREATEINFO_HPP

#include <Cocktail/Core/Flags.hpp>

#include <Cocktail/Renderer/Buffer/BufferUsage.hpp>
#include <Cocktail/Renderer/Memory/MemoryType.hpp>

namespace Ck::Renderer
{
    /**
     * \brief Buffer descriptor structure
     * Describe the set of parameters to create a Buffer
     * \see RenderDevice::CreateBuffer
     */
    struct BufferCreateInfo
    {
	    /**
         * \brief Specifies the usage to the Buffer to create
         */
        BufferUsageFlags Usage;

	    /**
         * \brief Specifies the size (in bytes) of the Buffer to create
         */
        std::size_t Size = 0;

	    /**
         * \brief Specifies the type of memory where store Buffer's data
         */
        MemoryType MemoryType = MemoryType::Static;

	    /**
         * \brief Specifies whether the buffer to create should be queue exclusive
         */
        bool Exclusive = false;

        /**
         * \brief Specifies the name of the Buffer to create
         * If the extension Debug is not supported by the RenderDevice creating the Buffer, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERCREATEINFO_HPP
