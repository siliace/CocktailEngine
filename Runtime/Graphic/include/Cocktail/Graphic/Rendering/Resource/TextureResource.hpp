#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP

#include <Cocktail/Graphic/Export.hpp>
#include <Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API TextureResource : public GraphicResource, public std::enable_shared_from_this<TextureResource>
	{
	public:

		/**
		 * \brief Constructor
		 */
		TextureResource(std::shared_ptr<GraphicEngine> graphicEngine, Extent3D<unsigned int> size, PixelFormat format, unsigned int layerCount, unsigned int levelCount, bool isCube = false, const AnsiChar* name = "");

		/**
		 * \brief 
		 * \param commandList 
		 */
		void GenerateMipMaps(Renderer::CommandList& commandList) const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Renderer::TextureView> GetView() const;

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
		std::shared_ptr<GraphicEngine> GetGraphicEngine() const override;

	private:

		std::shared_ptr<GraphicEngine> mGraphicEngine;
		std::shared_ptr<Renderer::Texture> mTexture;
		std::shared_ptr<Renderer::TextureView> mView;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
