#ifndef COCKTAIL_GRAPHIC_SCENE_SHAPE_SHAPE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SHAPE_SHAPE_HPP

#include <Cocktail/Core/Math/Volume/Volume.hpp>

#include <Cocktail/Graphic/Rendering/Queue/RenderQueue.hpp>
#include <Cocktail/Graphic/Spatial/Transformation.hpp>

namespace Ck
{
    /**
     * \brief Interface for renderable shapes in the scene graph
     *
     * This class represents an abstract renderable element that can be attached
     * to a scene graph node. Implementations are responsible for submitting their
     * render commands to the render queue and for contributing to spatial queries
     * such as bounding volume computation.
     */
    class Shape
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~Shape() = default;

        /**
         * \brief Submits the shape to the render queue
         *
         * This method is called during the rendering phase to allow the shape
         * to enqueue all necessary draw commands. The final transformation of
         * the shape is typically derived from the combination of the camera
         * and world transformations.
         *
         * \param queue The render queue to which render commands are added
         * \param cameraTransformation The transformation associated with the camera
         * \param worldTransformation The world transformation of the scene graph node
         */
        virtual void AddToQueue(RenderQueue& queue, const Transformation& cameraTransformation, const Transformation& worldTransformation) = 0;

        /**
         * \brief Extends a bounding volume to include this shape
         *
         * This method updates the given bounding volume so that it encloses
         * the spatial extent of the shape. It is typically used for culling,
         * collision detection, or other spatial queries.
         *
         * \param volume The bounding volume to extend
         */
        virtual void ExtendBoundingVolume(Volume<float>& volume) = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_SHAPE_SHAPE_HPP
