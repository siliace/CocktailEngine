#include <Cocktail/Graphic/Scene/Shape/StaticMeshShape.hpp>

namespace Ck
{
    StaticMeshShape::StaticMeshShape(GraphicEngine& graphicEngine, SharedPtr<Mesh> mesh, const Array<SharedPtr<Material>>& materials) :
        mMesh(std::move(mesh))
    {
        mMaterials.reserve(materials.GetSize());

        mVertexBuffer = graphicEngine.CreateVertexBuffer(mMesh->GetVertices());
        if (mMesh->IsIndexed())
            mIndexBuffer = graphicEngine.CreateIndexBuffer(mMesh->GetIndices());

        for (const Mesh::SubMesh& subMesh : mMesh->GetSubMeshes())
        {
            unsigned int materialIndex = subMesh.MaterialIndex;

            SharedPtr<Material> material = materials[materialIndex];

            mMaterials.insert(material);
            mGeometries[material.Get()].Add({ subMesh.Count, subMesh.FirstVertex, subMesh.FirstIndex, subMesh.PrimitiveTopology });
        }
    }

    void StaticMeshShape::AddToQueue(RenderQueue& queue, const Transformation& cameraTransformation, const Transformation& worldTransformation)
    {
		float distanceToCamera = Vector3<float>::DistanceBetween(worldTransformation.GetTranslation(), cameraTransformation.GetTranslation());

        StaticMeshRecordInfo recordInfo;
        recordInfo.Model = worldTransformation.ToMatrix();

        if (mIndexBuffer)
        {
            recordInfo.IndexBuffer = mIndexBuffer->GetUnderlyingResource();
            recordInfo.IndexType = mIndexBuffer->GetIndexArray()->GetIndexType();
            recordInfo.IndexBufferOffset = 0;
        }

        recordInfo.VertexBufferCount = 1;
        recordInfo.VertexBuffers[0].Buffer = mVertexBuffer->GetUnderlyingResource();
        recordInfo.VertexBuffers[0].Offset = 0;
        recordInfo.VertexBuffers[0].VertexLayout = mVertexBuffer->GetVertexArray()->GetVertexLayout().Get();

        for (const SharedPtr<Material>& material : mMaterials)
        {
            if (material->GetAlphaMode() == Material::AlphaMode::Opaque && queue.GetBlendingMode() == RenderQueue::BlendingMode::Transparent)
                continue;

            if (material->GetAlphaMode() != Material::AlphaMode::Opaque && queue.GetBlendingMode() == RenderQueue::BlendingMode::Opaque)
                continue;

            recordInfo.AlphaMode = material->GetAlphaMode();
            recordInfo.AlphaCutoff = material->GetAlphaCutoff();
            recordInfo.DoubleSided = material->IsDoubleSided();
            recordInfo.MaterialBaseColor = material->GetParameters().Base;
            recordInfo.MaterialEmissiveColor = material->GetParameters().Emission;
            recordInfo.MaterialMetallic = material->GetParameters().Metallic;
            recordInfo.MaterialRoughness = material->GetParameters().Roughness;
            for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
            {
                if (SharedPtr<TextureResource> texture = material->GetTexture(textureType))
                    recordInfo.MaterialTextures[textureType] = texture->GetView();
            }

            mGeometries.TryGet(material.Get()).Then([&](const Array<Geometry>& geometries) {
                for (const Geometry& geometry : geometries)
                {
                    recordInfo.Count = geometry.Count;
                    recordInfo.FirstVertex = geometry.FirstVertex;
                    recordInfo.FirstIndex = geometry.FirstIndex;

                    EmitRecord(queue, recordInfo, material->GetShadingMode(), distanceToCamera);
                }
            });
        }
    }

    void StaticMeshShape::ExtendBoundingVolume(Volume<float>& volume)
    {
        SharedPtr<VertexArray> vertices = mVertexBuffer->GetVertexArray();
        const SharedPtr<VertexLayout>& vertexLayout = vertices->GetVertexLayout();

        const VertexAttribute* positionAttribute = vertexLayout->FindAttribute(VertexAttributeSemantic::Position);
        if (positionAttribute)
        {
            const bool isFlat = positionAttribute->GetElementCount() == 2;
            assert(isFlat || positionAttribute->GetElementCount() == 3);

            for (std::size_t i = 0; i < vertices->GetVertexCount(); i++)
            {
                VertexRef vertex = vertices->At(i);
                const float* vertexPosition = static_cast<const float*>(vertex.Get(VertexAttributeSemantic::Position));

                Vector3<float> point;
                point.X() = vertexPosition[0];
                point.Y() = vertexPosition[1];
                point.Z() = !isFlat ? vertexPosition[2] : 0.f;

                volume.Extend(point);
            }
        }
    }

    void StaticMeshShape::EmitRecord(RenderQueue& queue, const StaticMeshRecordInfo& staticMeshRecordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey)
    {
        queue.PushStaticMesh(staticMeshRecordInfo, shadingMode, sortingKey);
    }
}
