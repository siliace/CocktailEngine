#include <Cocktail/Graphic/Scene/SceneNode.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

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

	Box<float> SceneNode::ComputeBoundingBox() const
	{
		Vector3<float> min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min());
		for (Ref<Shape> shape : mShapes)
		{
			Ref<Mesh> mesh = shape->GetMesh();
			Ref<VertexArray> vertices = mesh->GetVertexBuffer()->GetVertexArray();
			for (std::size_t i = 0; i < vertices->GetVertexCount(); i++)
			{
				VertexRef vertex = vertices->At(i);
				const float* position = vertex.Get<float>(VertexAttributeSemantic::Position);

				min.X() = std::min(min.X(), position[0]); max.X() = std::max(max.X(), position[0]);
				min.Y() = std::min(min.Y(), position[1]); max.Y() = std::max(max.Y(), position[1]);
				min.Z() = std::min(min.Z(), position[2]); max.Z() = std::max(max.Z(), position[2]);
			}
		}

		const Transformation& worldTransformation = GetWorldTransformation();
		Vector3<float> worldMin = worldTransformation.Apply(min);
		Vector3<float> worldMax = worldTransformation.Apply(max);

		return Box<float>::WithMinMax(worldMin, worldMax);
	}
}
