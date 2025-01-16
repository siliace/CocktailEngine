#ifndef COCKTAIL_RENDERER_BUFFER_BUFFER_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFER_HPP

#include <Cocktail/Core/Flags.hpp>

#include <Cocktail/Renderer/Buffer/BufferUsage.hpp>
#include <Cocktail/Renderer/Memory/MemoryResource.hpp>

namespace Ck::Renderer
{
	/**
     * \brief 
     */
    class Buffer : public Implements<Buffer, MemoryResource>
    {
    public:

	    /**
         * \brief 
         * \return 
         */
        virtual BufferUsageFlags GetUsage() const = 0;

    	/**
         * \brief 
         * \return 
         */
        virtual std::size_t GetSize() const = 0;
    };
}
#endif // COCKTAIL_RENDERER_BUFFER_BUFFER_HPP
