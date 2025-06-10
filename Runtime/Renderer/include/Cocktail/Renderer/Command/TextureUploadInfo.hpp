#ifndef COCKTAIL_RENDERER_COMMAND_TEXTUREUPLOADINFO_HPP
#define COCKTAIL_RENDERER_COMMAND_TEXTUREUPLOADINFO_HPP

#include <Cocktail/Core/Extent3D.hpp>

namespace Ck::Renderer
{
	/**
     * \brief Structure describing a texture upload
     * This structure specifies where pixels should be copied in the receiving Texture
     */
    struct TextureUploadInfo
    {
	    /**
         * \brief Specifies the layer to upload
         */
        unsigned int ArrayLayer = 0;

	    /**
         * \brief Specifies the level to upload
         */
        unsigned int Level = 0;

	    /**
         * \brief Specifies the size in pixels of the upload
         */
        Extent3D<unsigned int> Size;

	    /**
         * \brief Specifies the offset where should be copied uploaded pixels
         */
        Extent3D<unsigned int> Offset;

	    /**
         * \brief Pixels to upload
         */
        const void* Pixels = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_COMMAND_TEXTUREUPLOADINFO_HPP
