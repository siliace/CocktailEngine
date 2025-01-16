#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Export.hpp>
#include <Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API TextureResource : public Inherit<TextureResource, Object, GraphicResource>
	{
	public:

		/**
		 * \brief Constructor
		 */
		TextureResource(Ref<GraphicEngine> graphicEngine, Extent3D<unsigned int> size, PixelFormat format, unsigned int layerCount, unsigned int levelCount, bool isCube = false, std::string_view name = "");

		/**
		 * \brief 
		 * \param commandList 
		 */
		void GenerateMipMaps(Renderer::CommandList& commandList) const;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::TextureView> GetView() const;
		/**
		 * \brief Get the TextureType the Texture was created with
		 * \return The TextureType
		 * \see TextureCreateInfo::Type
		 */
		Renderer::TextureType GetType() const;

		/**
		 * \brief Get the Format the Texture was created with
		 * \return The format
		 * \see TextureCreateInfo::Format
		 */
		PixelFormat GetFormat() const;

		/**
		 * \brief Get the size the Texture was created with
		 * \return The size
		 * \see TextureCreateInfo::Size
		 */
		Extent3D<unsigned int> GetSize() const;

		/**
		 * \brief Get the number of mipmap levels of the Texture
		 * \return The level count
		 * \see TextureCreateInfo::MipMapsCount
		 */
		unsigned int GetMipMapCount() const;

		/**
		 * \brief Get the number of array layers of the Texture
		 * \return The layer count
		 * \see TextureCreateInfo::ArrayLayerCount
		 */
		unsigned int GetArrayLayerCount() const;

		/**
		 * \brief
		 * \return
		 */
		Renderer::Texture* GetUnderlyingResource() const override;

		/**
		 * \brief
		 * \return 
		 */
		Ref<GraphicEngine> GetGraphicEngine() const override;

	private:

		Ref<GraphicEngine> mGraphicEngine;
		Ref<Renderer::Texture> mTexture;
		Ref<Renderer::TextureView> mView;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
