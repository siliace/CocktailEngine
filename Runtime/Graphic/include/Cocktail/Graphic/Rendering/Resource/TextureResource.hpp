#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Export.hpp>
#include <Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API TextureResource : public Inherit<TextureResource, Object, GraphicResource>
	{
	public:

		/**
		 * \brief 
		 * \param commandList 
		 */
		virtual void GenerateMipMaps(Renderer::CommandList& commandList) const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Ref<Renderer::TextureView> GetView() const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Renderer::Texture* GetUnderlyingResource() const override = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_TEXTURERESOURCE_HPP
