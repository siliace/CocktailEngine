#ifndef COCKTAIL_RENDERER_TEXTURE_HPP
#define COCKTAIL_RENDERER_TEXTURE_HPP

#include <Cocktail/Core/Extent3D.hpp>

#include <Cocktail/Renderer/Memory/MemoryResource.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>

namespace Ck::Renderer
{
    /**
     * \brief 
     */
    class Texture : public Implements<Texture, MemoryResource>
    {
    public:

        /**
         * \brief Get the TextureType the Texture was created with
         * \return The TextureType
         * \see TextureCreateInfo::Type
         */
        virtual TextureType GetType() const = 0;

	    /**
         * \brief Get the Format the Texture was created with
         * \return The format
         * \see TextureCreateInfo::Format
         */
        virtual PixelFormat GetFormat() const = 0;

        /**
         * \brief Get the size the Texture was created with
         * \return The size
         * \see TextureCreateInfo::Size
         */
        virtual Extent3D<unsigned int> GetSize() const = 0;

	    /**
         * \brief Get the number of mipmap levels of the Texture
         * \return The level count
         * \see TextureCreateInfo::MipMapsCount
         */
        virtual unsigned int GetMipMapCount() const = 0;

	    /**
         * \brief Get the number of array layers of the Texture
         * \return The layer count
         * \see TextureCreateInfo::ArrayLayerCount
         */
        virtual unsigned int GetArrayLayerCount() const = 0;
    };
}

#endif // COCKTAIL_RENDERER_TEXTURE_HPP
