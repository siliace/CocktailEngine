#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP

#include <CocktailEngine/Graphic/Export.hpp>
#include <CocktailEngine/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <CocktailEngine/Renderer/Command/CommandList.hpp>
#include <CocktailEngine/Renderer/Texture/Texture.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_GRAPHIC_API TextureResource : public GraphicResource, public SharedFromThis<TextureResource>
	{
	public:

		/**
		 * \brief Constructor
		 */
		TextureResource(SharedPtr<GraphicEngine> graphicEngine, Extent3D<unsigned int> size, PixelFormat format, unsigned int layerCount, unsigned int levelCount, bool isCube = false, const AnsiChar* name = "");

		/**
		 * \brief 
		 * \param commandList 
		 */
		void GenerateMipMaps(Renderer::CommandList& commandList) const;

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<Renderer::TextureView> GetView() const;

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
		SharedPtr<GraphicEngine> GetGraphicEngine() const override;

	private:

		SharedPtr<GraphicEngine> mGraphicEngine;
		SharedPtr<Renderer::Texture> mTexture;
		SharedPtr<Renderer::TextureView> mView;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
