#ifndef COCKTAIL_RENDERER_COMMAND_TEXTUREUPLOADINFO_HPP
#define COCKTAIL_RENDERER_COMMAND_TEXTUREUPLOADINFO_HPP

#include <Cocktail/Core/Extent3D.hpp>

namespace Ck::Renderer
{
	/**
     * \brief 
     */
    struct TextureUploadInfo
    {
	    /**
         * \brief 
         */
        unsigned int ArrayLayer = 0;

	    /**
         * \brief 
         */
        unsigned int Level = 0;

	    /**
         * \brief 
         */
        Extent3D<unsigned int> Size;

	    /**
         * \brief 
         */
        Extent3D<unsigned int> Offset;

	    /**
         * \brief 
         */
        const void* Pixels = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_COMMAND_TEXTUREUPLOADINFO_HPP
