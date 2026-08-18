#ifndef COCKTAILENGINE_RENDERER_BUFFER_BUFFERUSAGE_HPP
#define COCKTAILENGINE_RENDERER_BUFFER_BUFFERUSAGE_HPP

#include <CocktailEngine/Core/Flags.hpp>

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

#endif // COCKTAILENGINE_RENDERER_BUFFER_BUFFERUSAGE_HPP
