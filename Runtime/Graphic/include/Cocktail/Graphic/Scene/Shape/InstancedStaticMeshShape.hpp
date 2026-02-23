#ifndef COCKTAIL_GRAPHIC_SCENE_SHAPE_INSTANCEDSTATICMESHSHAPE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SHAPE_INSTANCEDSTATICMESHSHAPE_HPP

#include <Cocktail/Graphic/Scene/Shape/StaticMeshShape.hpp>
#include <Cocktail/Graphic/Rendering/Resource/StorageBuffer.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API InstancedStaticMeshShape : public StaticMeshShape
    {
    public:

        struct InstanceInfo
        {
            Matrix4<float> ModelMatrix;
            Vector4<float> NormalMatrix[3];
        };

        /**
         * \brief
         *
         * \param graphicEngine The graphic engine used to create rendering resources
         * \param instances
         * \param mesh The static mesh to render
         * \param materials The set of materials applied to the mesh
         */
        InstancedStaticMeshShape(GraphicEngine& graphicEngine, const Array<Transformation>& instances, std::shared_ptr<Mesh> mesh, const Array<std::shared_ptr<Material>>& materials);

        /**
         * \brief
         *
         * \param volume
         */
        void ExtendBoundingVolume(Volume<float>& volume) override;

    protected:

        void EmitRecord(RenderQueue& queue, const StaticMeshRecordInfo& staticMeshRecordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey) override;

    private:

        Array<Transformation> mInstances;
        std::shared_ptr<TypedStorageBuffer<InstanceInfo>> mInstanceBuffer;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_SHAPE_INSTANCEDSTATICMESHSHAPE_HPP
