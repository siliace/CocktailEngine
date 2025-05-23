#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTURECREATEINFO_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTURECREATEINFO_HPP

#include <Cocktail/Core/Extent3D.hpp>
#include <Cocktail/Core/Flags.hpp>
#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/Memory/MemoryType.hpp>

namespace Ck::Renderer
{
	/**
     * \brief 
     */
    enum class TextureType
    {
	    /**
         * \brief 
         */
        e1D,

        /**
         * \brief 
         */
        e2D,

        /**
         * \brief 
         */
        e3D
    };

    /**
     * \brief 
     */
    enum class TextureCreateFlagBits
    {
        /**
         * \brief Specifies that the Texture can be used to create a TextureView of type TextureViewType::Cube or TextureViewType::CubeArray
         * This flag can be applied only on a texture with type TextureType::e2D
         */
        CubemapCompatible = Bit(0),

        /**
         * \brief Specifies that the Texture can be used to create a TextureView of type TextureViewType::Array2D
         * This flag can be applied only on a texture with type TextureType::e3D
         * If the extension TextureView2DArrayCompatible is not supported, this flags is ignored
         */
        Array2DCompatible = Bit(1),
    };

    using TextureCreateFlags = Flags<TextureCreateFlagBits>;

    /**
     * \brief 
     */
    enum class TextureUsageFlagBits
    {
        /**
         * \brief Specifies the Texture can be sampled from a shader
         */
        Sampled = Bit(0),

        /**
         * \brief Specifies the Texture can be written from a shader
         */
        Storage = Bit(1),

        /**
         * \brief Specifies that the Texture can be used to create a TextureView suitable as attachment in a RenderTarget
         */
        Attachment = Bit(2)
    };

    using TextureUsageFlags = Flags<TextureUsageFlagBits>;

	/**
	 * \brief Mutable format descriptor structure
	 * Describe the formats a Texture must be compatible with
	 * \see TextureCreateInfo
	 */
	struct TextureMutableFormatCreateInfo
	{
		/**
		 * \brief Specifies the list of formats the Texture must be compatible with
		 */
		const PixelFormat* CompatibleFormats = nullptr;

		/**
		 * \brief Specifies the number compatible formats
		 */
		unsigned int CompatibleFormatCount = 0;
	};

    /**
     * \brief Texture descriptor structure
     * Describe the set of parameters to create a Texture
     * \see RenderDevice::CreateTexture
     */
    struct TextureCreateInfo
    {
        /**
         * \brief Specifies flags to apply to the Texture to create
         */
        TextureCreateFlags Flags;

        /**
         * \brief Specifies the type the Texture to create
         */
        TextureType Type = TextureType::e1D;

        /**
         * \brief Specifies the format of the Texture to create
         */
        PixelFormat Format;

        /**
         * \brief Specifies the size in texel of the Texture to create
         * If the \p Type is neither TextureType::e2D nor TextureType::e3D, the height parameter is ignored
         * If the \p Type is not TextureType::e3D, the depth parameter is ignored
         */
        Extent3D<unsigned int> Size;

        /**
         * \brief Specifies usages of the Texture to create
         */
        TextureUsageFlags Usage;

        /**
         * \brief Specifies the type of memory where store Texture's texels
         */
        MemoryType MemoryType = MemoryType::Static;

        /**
         * \brief Specifies whether the texture to create should be queue exclusive
         */
        bool Exclusive = false;
        
        /**
         * \brief Specifies the size of the mipmap chain of the Texture to create
         * If this value is 0, the created texture will have the longest mipmap chain possible
         */
        unsigned int MipMapsCount = 1;

        /**
         * \brief Specifies the size of the array of the Texture to create
         * If the \p Type is TextureType::e3D, this parameter is ignored
         * If the \p Flags contains TextureCreateFlagBits::CubemapCompatible is value must be a multiple of 6
         */
        unsigned int ArrayLayerCount = 1;

        /**
		 * \brief Specifies the parameters of mutable formats of the Texture to create
		 */
		TextureMutableFormatCreateInfo* MutableFormat = nullptr;

        /**
         * \brief Specifies the name of the Texture to create
         * If the extension Debug is not supported by the RenderDevice creating the Texture, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_TEXTURE_TEXTURECREATEINFO_HPP
