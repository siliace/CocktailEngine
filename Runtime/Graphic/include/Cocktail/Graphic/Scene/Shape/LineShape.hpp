#ifndef COCKTAIL_GRAPHIC_SCENE_SHAPE_LINESHAPE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SHAPE_LINESHAPE_HPP

#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Shape/Shape.hpp>

namespace Ck
{
    /**
     * \brief Shape implementation for rendering line primitives
     *
     * LineShape represents a collection of line segments or line strips that can
     * be attached to a scene graph node. It is typically used for debug rendering,
     * gizmos, or simple geometric visualizations.
     */
    class COCKTAIL_GRAPHIC_API LineShape : public Shape
    {
    public:

        /**
         * \brief Constructs a line shape with a uniform color
         *
         * Creates a line shape from a list of points using a single color for
         * all vertices. The lines can be rendered either as independent segments
         * or as a continuous strip.
         *
         * \param graphicEngine The graphic engine used to create rendering resources
         * \param points The points defining the line vertices
         * \param color The color applied to all line vertices
         * \param thickness The thickness of the rendered lines
         * \param isStrip Indicates whether the points form a line strip or independent line segments
         */
        LineShape(GraphicEngine& graphicEngine, Array<Vector3<float>> points, LinearColor color, float thickness, bool isStrip);

        /**
         * \brief Constructs a line shape with per-vertex colors
         *
         * Creates a line shape from a list of points with individual colors per
         * vertex. A fallback color can be provided and used if needed by the
         * rendering pipeline.
         *
         * \param graphicEngine The graphic engine used to create rendering resources
         * \param points The points defining the line vertices
         * \param colors The per-vertex colors associated with each point
         * \param color A fallback or default color
         * \param thickness The thickness of the rendered lines
         * \param isStrip Indicates whether the points form a line strip or independent line segments
         */
        LineShape(GraphicEngine& graphicEngine, Array<Vector3<float>> points, Array<LinearColor> colors, LinearColor color, float thickness, bool isStrip);

        /**
         * \brief Submits the line primitives to the render queue
         *
         * Adds the line draw commands to the render queue using the appropriate
         * vertex buffer and line topology. Transformations are combined with the
         * camera and scene graph state.
         *
         * \param queue The render queue to which draw commands are added
         * \param cameraTransformation The transformation associated with the camera
         * \param worldTransformation The world transformation of the scene graph node
         */
        void AddToQueue(RenderQueue& queue, const Transformation& cameraTransformation, const Transformation& worldTransformation) override;

        /**
         * \brief Extends a bounding volume to include the line shape
         *
         * Updates the given bounding volume so that it encloses all points
         * defining the line primitives.
         *
         * \param volume The bounding volume to extend
         */
        void ExtendBoundingVolume(Volume<float>& volume) override;

    private:

        unsigned int mPointCount; /*!< Number of points defining the line shape */
        LinearColor mColor; /*!< Default or uniform color used for rendering */
        float mThickness; /*!< Thickness of the rendered lines */
        bool mIsStip; /*!< Indicates whether the lines are rendered as a strip or as segments */
        std::shared_ptr<VertexBuffer> mVertexBuffer; /*!< GPU vertex buffer containing the line vertices */
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_SHAPE_LINESHAPE_HPP
