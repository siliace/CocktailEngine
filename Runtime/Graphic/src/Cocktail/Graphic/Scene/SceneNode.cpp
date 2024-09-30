#include <Cocktail/Graphic/Scene/SceneNode.hpp>

#include "Cocktail/Graphic/Rendering/Resource/TextureResource.hpp"

namespace Ck
{
	SceneNode::SceneNode(Ref<TransformationNode> transformationNode) :
		Inherit(std::move(transformationNode))
	{
		/// Nothing
	}

	void SceneNode::AddShape(Ref<Shape> shape)
	{
		mShapes.emplace_back(std::move(shape));
	}

	void SceneNode::AddToQueue(RenderQueue& queue)
	{
		for (Ref<Shape> shape : mShapes)
		{
			StaticMeshRecordInfo recordInfo;

			Ref<Material> material = shape->GetMaterial();
			if (material->GetShadingMode() != queue.GetShadingMode())
				continue;

			recordInfo.DoubleSided = material->IsDoubleSided();
			recordInfo.UniformColors = material->GetUniformColors();
			for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
			{
				if (Ref<TextureResource> texture = material->GetTexture(textureType))
					recordInfo.MaterialTextures[textureType] = texture->GetView();
			}

			Ref<Mesh> mesh = shape->GetMesh();
			Ref<VertexBuffer> vertexBuffer = mesh->GetVertexBuffer();
			Ref<VertexArray> vertices = vertexBuffer->GetVertexArray();

			if (Ref<IndexBuffer> indexBuffer = mesh->GetIndexBuffer())
			{
				Ref<IndexArray> indices = indexBuffer->GetIndexArray();
				recordInfo.Count = static_cast<unsigned int>(indices->GetIndexCount());
				recordInfo.IndexBuffer = indexBuffer->GetUnderlyingResource();
				recordInfo.IndexType = indices->GetIndexType();
				recordInfo.IndexBufferOffset = 0;
			}
			else
			{
				recordInfo.Count = static_cast<unsigned int>(vertices->GetVertexCount());
			}

			recordInfo.VertexBufferCount = 1;
			recordInfo.VertexBuffers[0].Buffer = vertexBuffer->GetUnderlyingResource();
			recordInfo.VertexBuffers[0].Offset = 0;
			recordInfo.VertexBuffers[0].VertexLayout = vertices->GetVertexLayout().Get();
			recordInfo.PrimitiveTopology = mesh->GetPrimitiveTopology();
			recordInfo.Model = GetTransformationNode()->GetWorldTransformation().ToMatrix();
			
			queue.PushStaticMesh(recordInfo);
		}
	}
}
