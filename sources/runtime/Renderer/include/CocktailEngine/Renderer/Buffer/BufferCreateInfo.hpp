#ifndef COCKTAILENGINE_RENDERER_BUFFER_BUFFERCREATEINFO_HPP
#define COCKTAILENGINE_RENDERER_BUFFER_BUFFERCREATEINFO_HPP

#include <CocktailEngine/Core/Flags.hpp>

#include <CocktailEngine/Renderer/Buffer/BufferUsage.hpp>
#include <CocktailEngine/Renderer/Memory/MemoryPriority.hpp>
#include <CocktailEngine/Renderer/Memory/MemoryType.hpp>

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
        MemoryType ResourceMemoryType = MemoryType::Static;

        /**
         * \brief Specifies the priority of the memory in the VRAM
         */
        MemoryPriority Priority = MemoryPriority::Medium;

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

#endif // COCKTAILENGINE_RENDERER_BUFFER_BUFFERCREATEINFO_HPP
