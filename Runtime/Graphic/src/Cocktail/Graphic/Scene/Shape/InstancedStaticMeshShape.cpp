#include <Cocktail/Core/Math/Matrix/Matrix3.hpp>

#include <Cocktail/Graphic/Scene/Shape/InstancedStaticMeshShape.hpp>
#include <Cocktail/Graphic/Rendering/Queue/InstancedStaticMeshRecordInfo.hpp>

namespace Ck
{
    InstancedStaticMeshShape::InstancedStaticMeshShape(GraphicEngine& graphicEngine, const Array<Transformation>& instances, std::shared_ptr<Mesh> mesh, const Array<std::shared_ptr<Material>>& materials) :
        StaticMeshShape(graphicEngine, std::move(mesh), materials),
        mInstances(instances)
    {
        mInstanceBuffer = graphicEngine.CreateTypedStorageBuffer<InstanceInfo>(mInstances.GetSize());
        for (unsigned int i = 0; i < mInstances.GetSize(); i++)
        {
            InstanceInfo& instanceInfo = mInstanceBuffer->At(i);
            instanceInfo.ModelMatrix = mInstances[i].ToMatrix().Transpose();
            Matrix3<float> normalMatrix = Matrix3<float>::From(instanceInfo.ModelMatrix).Inverse().Transpose();
            for (unsigned int i = 0; i < 3; i++)
                instanceInfo.NormalMatrix[i] = Vector4<float>(normalMatrix.GetColumn(i).Normalized(), 0.f);
        }

        mInstanceBuffer->Upload();
    }

    void InstancedStaticMeshShape::ExtendBoundingVolume(Volume<float>& volume)
    {
        Box<float> instanceVolume;
        StaticMeshShape::ExtendBoundingVolume(instanceVolume);
        for (const Transformation& instanceTransformation : mInstances)
        {
            for (unsigned int j = 0; j < instanceVolume.GetVertexCount(); j++)
            {
                Vector3<float> vertex = instanceVolume.GetVertex(j);
                Vector3<float> transformedInstanceVertex = instanceTransformation.Apply(vertex);
                volume.Extend(transformedInstanceVertex);
            }
        }
    }

    void InstancedStaticMeshShape::EmitRecord(RenderQueue& queue, const StaticMeshRecordInfo& staticMeshRecordInfo, Material::ShadingMode shadingMode, Uint64 sortingKey)
    {
        InstancedStaticMeshRecordInfo instancedStaticMeshRecordInfo;
        instancedStaticMeshRecordInfo.StaticMeshRecord = staticMeshRecordInfo;
        instancedStaticMeshRecordInfo.InstancesBuffer = mInstanceBuffer->GetUnderlyingResource();
        instancedStaticMeshRecordInfo.InstanceBufferOffset = 0;
        instancedStaticMeshRecordInfo.InstanceCount = mInstances.GetSize();
        instancedStaticMeshRecordInfo.FirstInstance = 0;

        queue.PushInstancedStaticMesh(instancedStaticMeshRecordInfo, shadingMode, sortingKey);
    }
}
