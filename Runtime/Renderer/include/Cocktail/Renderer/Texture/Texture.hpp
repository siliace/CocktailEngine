#ifndef COCKTAIL_RENDERER_TEXTURE_HPP
#define COCKTAIL_RENDERER_TEXTURE_HPP

#include <Cocktail/Core/Extent3D.hpp>

#include <Cocktail/Renderer/Export.hpp>
#include <Cocktail/Renderer/Memory/MemoryResource.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>

namespace Ck::Renderer
{
    /**
     * \brief Interface representing an opaque handle to a GPU texture object
     * A Texture is a multidimensional array of data representing pixels.
     * Their purpose is to create TextureView objects that will be used in CommandList.
     */
    class Texture : public MemoryResource
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

    /**
     * \brief Compute the maximum number of mip maps level a texture with a given base size can support
     * \param baseSize The size of the first level of the texture
     * \return The number of level
     */
    COCKTAIL_RENDERER_API unsigned int ComputeTextureLevelCount(const Extent3D<unsigned int>& baseSize);

    /**
     * \brief Helper function to compute the size of a texture's mipmap
     * \param baseSize The size of the first level of the texture
     * \param level The level to compute
     * \return The size of the level
     */
    COCKTAIL_RENDERER_API Extent3D<unsigned int> ComputeTextureLevelSize(const Extent3D<unsigned int>& baseSize, unsigned int level);
}

#endif // COCKTAIL_RENDERER_TEXTURE_HPP
