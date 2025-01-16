#ifndef COCKTAIL_RENDERER_MEMORY_MEMORYTYPE_HPP
#define COCKTAIL_RENDERER_MEMORY_MEMORYTYPE_HPP

namespace Ck::Renderer
{
	/**
     * \brief 
     */
    enum class MemoryType
    {
	    /**
         * \brief
         */
        Static,

        /**
         * \brief 
         */
        Dynamic,

	    /**
         * \brief 
         */
        Streamed,
        
        /**
         * \brief 
         */
        Unified
    };
}

#endif // COCKTAIL_RENDERER_MEMORY_MEMORYTYPE_HPP
