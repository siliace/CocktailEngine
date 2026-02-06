#ifndef COCKTAIL_GRAPHIC_SCENE_SHAPE_STATICMESHSHAPE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SHAPE_STATICMESHSHAPE_HPP

#include <Cocktail/Graphic/Scene/Shape/Shape.hpp>

#include "Cocktail/Graphic/Mesh/Mesh.hpp"
#include "Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp"

namespace Ck
{
    /**
     * \brief Shape implementation for rendering static meshes
     *
     * StaticMeshShape represents a non-animated mesh that can be attached
     * to a scene graph node. It submits one or more geometries to the render
     * queue, grouped by material, and relies on prebuilt vertex and index
     * buffers for efficient rendering.
     */
    class COCKTAIL_GRAPHIC_API StaticMeshShape : public Shape
    {
    public:

        /**
         * \brief Constructs a StaticMeshShape
         *
         * Initializes GPU resources required to render the given mesh and
         * associates the provided materials with the corresponding geometries.
         *
         * \param graphicEngine The graphic engine used to create rendering resources
         * \param mesh The static mesh to render
         * \param materials The set of materials applied to the mesh
         */
        StaticMeshShape(GraphicEngine& graphicEngine, std::shared_ptr<Mesh> mesh, const Array<std::shared_ptr<Material>>& materials);

        /**
         * \brief Submits the mesh geometries to the render queue
         *
         * For each material, the corresponding geometries are added to the render
         * queue using the appropriate vertex/index buffers and primitive topology.
         * Transformations are combined with the scene graph and camera state.
         *
         * \param queue The render queue to which draw commands are added
         * \param cameraTransformation The transformation associated with the camera
         * \param worldTransformation The world transformation of the scene graph node
         */
        void AddToQueue(RenderQueue& queue, const Transformation& cameraTransformation, const Transformation& worldTransformation) override;

        /**
         * \brief Extends a bounding volume to include the mesh
         *
         * Updates the given bounding volume so that it encloses the spatial
         * extent of the static mesh in world space.
         *
         * \param volume The bounding volume to extend
         */
        void ExtendBoundingVolume(Volume<float>& volume) override;

    private:

        struct Geometry
        {
            unsigned int Count = 0;
            unsigned int FirstVertex = 0;
            unsigned int FirstIndex = 0;
            Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
        };

        std::shared_ptr<Mesh> mMesh;
        std::shared_ptr<VertexBuffer> mVertexBuffer;
        std::shared_ptr<IndexBuffer> mIndexBuffer;
        std::unordered_set<std::shared_ptr<Material>> mMaterials;
        std::unordered_map<const Material*, Array<Geometry>> mGeometries;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_SHAPE_STATICMESHSHAPE_HPP
