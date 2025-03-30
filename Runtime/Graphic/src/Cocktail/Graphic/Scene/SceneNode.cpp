#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneNode.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

namespace Ck
{
	Shape::Shape(GraphicEngine& graphicEngine, Ref<Mesh> mesh, const std::vector<Ref<Material>>& materials):
		mMesh(std::move(mesh))
	{
		mMaterials.reserve(materials.size());

		mVertexBuffer = graphicEngine.CreateVertexBuffer(mMesh->GetVertices());
		if (mMesh->IsIndexed())
			mIndexBuffer = graphicEngine.CreateIndexBuffer(mMesh->GetIndices());

		for (const Mesh::SubMesh& subMesh : mMesh->GetSubMeshes())
		{
			unsigned int materialIndex = subMesh.MaterialIndex;

			Ref<Material> material = materials[materialIndex];

			mMaterials.insert(material);
			mGeometries[material.Get()].push_back({ subMesh.Count, subMesh.FirstVertex, subMesh.FirstIndex, subMesh.PrimitiveTopology });
		}
	}

	const Ref<VertexBuffer>& Shape::GetVertexBuffer() const
	{
		return mVertexBuffer;
	}

	const Ref<IndexBuffer>& Shape::GetIndexBuffer() const
	{
		return mIndexBuffer;
	}

	const std::unordered_set<Ref<Material>>& Shape::GetMaterials() const
	{
		return mMaterials;
	}

	const std::vector<Shape::Geometry>& Shape::GetGeometries(const Material* material) const
	{
		assert(material != nullptr);
		return mGeometries.at(material);
	}

	SceneNode::SceneNode(Scene* scene, Ref<TransformationNode> transformationNode) :
		Inherit(std::move(transformationNode)),
		mScene(scene),
		mVisible(true)
	{
		/// Nothing
	}

	void SceneNode::AddShape(Ref<Shape> shape)
	{
		mShapes.emplace_back(std::move(shape));
	}

	void SceneNode::AddToQueue(RenderQueue& queue, const Camera& camera)
	{
		assert(mVisible);

		Transformation cameraTransformation = camera.GetWorldTransformation();
		const Transformation& worldTransformation = GetTransformationNode()->GetWorldTransformation();
		float distanceToCamera = Vector3<float>::DistanceBetween(worldTransformation.GetTranslation(), cameraTransformation.GetTranslation());

		StaticMeshRecordInfo recordInfo;
		recordInfo.Model = worldTransformation.ToMatrix();

		for (Ref<Shape> shape : mShapes)
		{
			Ref<VertexBuffer> vertexBuffer = shape->GetVertexBuffer();

			if (Ref<IndexBuffer> indexBuffer = shape->GetIndexBuffer())
			{
				recordInfo.IndexBuffer = indexBuffer->GetUnderlyingResource();
				recordInfo.IndexType = indexBuffer->GetIndexArray()->GetIndexType();
				recordInfo.IndexBufferOffset = 0;
			}

			recordInfo.VertexBufferCount = 1;
			recordInfo.VertexBuffers[0].Buffer = vertexBuffer->GetUnderlyingResource();
			recordInfo.VertexBuffers[0].Offset = 0;
			recordInfo.VertexBuffers[0].VertexLayout = vertexBuffer->GetVertexArray()->GetVertexLayout().Get();

			for (const Ref<Material>& material : shape->GetMaterials())
			{
				if (material->GetShadingMode() != queue.GetShadingMode())
					continue;

				if (material->IsOpaque() && queue.GetBlendingMode() == RenderQueue::BlendingMode::Transparent)
					continue;

				if (!material->IsOpaque() && queue.GetBlendingMode() == RenderQueue::BlendingMode::Opaque)
					continue;

				recordInfo.Opaque = material->IsOpaque();
				recordInfo.DoubleSided = material->IsDoubleSided();
				recordInfo.MaterialBaseColor = material->GetUniformColors().Base;
				recordInfo.MaterialSpecularColor = material->GetUniformColors().Specular;
				recordInfo.MaterialEmissiveColor = material->GetUniformColors().Emission;
				recordInfo.Sampler = mScene->GetGraphicEngine()->GetSampler(material->GetSamplerType());
				for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
				{
					if (Ref<TextureResource> texture = material->GetTexture(textureType))
						recordInfo.MaterialTextures[textureType] = texture->GetView();
				}

				for (const Shape::Geometry& geometry : shape->GetGeometries(material.Get()))
				{
					recordInfo.Count = geometry.Count;
					recordInfo.FirstVertex = geometry.FirstVertex;
					recordInfo.FirstIndex = geometry.FirstIndex;

					queue.PushStaticMesh(recordInfo, distanceToCamera);
				}
			}
		}
	}

	Box<float> SceneNode::ComputeBoundingBox() const
	{
		Vector3<float> min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min());
		for (Ref<Shape> shape : mShapes)
		{
			Ref<VertexArray> vertices = shape->GetVertexBuffer()->GetVertexArray();
			for (std::size_t i = 0; i < vertices->GetVertexCount(); i++)
			{
				VertexRef vertex = vertices->At(i);
				const float* position = static_cast<const float*>(vertex.Get(VertexAttributeSemantic::Position));

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

	bool SceneNode::IsVisible() const
	{
		return mVisible;
	}

	void SceneNode::SetVisible(bool visible)
	{
		mVisible = visible;
	}
}
