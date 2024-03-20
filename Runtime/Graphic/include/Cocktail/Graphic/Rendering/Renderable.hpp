#ifndef COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class Renderable : public Implements<Renderable, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param queue 
		 */
		virtual void AddToQueue(RenderQueue& queue) = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP
