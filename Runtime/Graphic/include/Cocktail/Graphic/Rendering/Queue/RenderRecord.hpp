#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERRECORD_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERRECORD_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API RenderRecord : public Implements<RenderRecord, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param commandList
		 * \param drawContext 
		 */
		virtual void Draw(Renderer::CommandList& commandList, RecordDrawContext& drawContext) const = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RENDERRECORD_HPP
