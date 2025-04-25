#ifndef COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>
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
		 * \param camera
		 */
		virtual void AddToQueue(RenderQueue& queue, const Camera& camera) = 0;

		/**
		 * \brief Get the volume encompassing every point of the object
		 * \return The bounding volume
		 */
		virtual const Volume<float>& GetBoundingVolume() const = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP
