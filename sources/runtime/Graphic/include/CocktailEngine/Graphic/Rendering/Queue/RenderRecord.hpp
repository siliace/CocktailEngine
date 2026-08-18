#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERRECORD_HPP

#include <CocktailEngine/Graphic/Export.hpp>

#include <CocktailEngine/Renderer/Command/CommandList.hpp>

namespace Ck
{
    class RecordDrawContext;

	/**
	 * \brief 
	 */
	class COCKTAILENGINE_GRAPHIC_API RenderRecord
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~RenderRecord() = default;

		/**
		 * \brief 
		 * \param commandList
		 * \param drawContext 
		 */
		virtual void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERRECORD_HPP
