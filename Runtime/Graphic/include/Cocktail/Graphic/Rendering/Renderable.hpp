#ifndef COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP

#include <Cocktail/Core/Math/Volume/Volume.hpp>

namespace Ck
{
    class Camera;
    class RenderQueue;

	/**
	 * \brief 
	 */
	class Renderable
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~Renderable() = default;

		/**
		 * \brief 
		 * \param queue 
		 * \param camera
		 */
		virtual void AddToQueue(RenderQueue& queue, const Camera* camera) = 0;

		/**
		 * \brief Get the volume encompassing every point of the object
		 * \return The bounding volume
		 */
		virtual const Volume<float>& GetBoundingVolume() const = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RENDERABLE_HPP
