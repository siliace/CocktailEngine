#ifndef COCKTAIL_RENDERER_BUFFER_BUFFERUSAGE_HPP
#define COCKTAIL_RENDERER_BUFFER_BUFFERUSAGE_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck::Renderer
{
	/**
     * \brief 
     */
    enum class BufferUsageFlagBits
    {
	    /**
         * \brief 
         */
        Vertex = Bit(0),

	    /**
         * \brief 
         */
        Index = Bit(1),

	    /**
         * \brief 
         */
        Uniform = Bit(2),

        /**
         * \brief 
         */
        Storage = Bit(3),

    	/**
         * \brief 
         */
        Indirect = Bit(4),
    };

    using BufferUsageFlags = Flags<BufferUsageFlagBits>;
};

#endif // COCKTAIL_RENDERER_BUFFER_BUFFERUSAGE_HPP
