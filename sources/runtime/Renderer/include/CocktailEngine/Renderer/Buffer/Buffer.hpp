#ifndef COCKTAILENGINE_RENDERER_BUFFER_BUFFER_HPP
#define COCKTAILENGINE_RENDERER_BUFFER_BUFFER_HPP

#include <CocktailEngine/Core/Flags.hpp>

#include <CocktailEngine/Renderer/Buffer/BufferUsage.hpp>
#include <CocktailEngine/Renderer/Memory/MemoryResource.hpp>

namespace Ck::Renderer
{
	/**
     * \brief Interface representing an opaque handle to a GPU buffer object
     * A Buffer is linear arrays of data which are used for various purposes by binding them in a CommandList.
     */
    class Buffer : public MemoryResource
    {
    public:

	    /**
         * \brief Get the usage of the Buffer
         * \return The usage of the Buffer
         */
        virtual BufferUsageFlags GetUsage() const = 0;

    	/**
         * \brief Get the size of the Buffer
         * \return The size of the Buffer
         */
        virtual std::size_t GetSize() const = 0;
    };
}
#endif // COCKTAILENGINE_RENDERER_BUFFER_BUFFER_HPP
