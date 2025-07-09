#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneNode.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

namespace Ck
{
	Shape::Shape(GraphicEngine& graphicEngine, std::shared_ptr<Mesh> mesh, const Array<std::shared_ptr<Material>>& materials):
		mMesh(std::move(mesh))
	{
		mMaterials.reserve(materials.GetSize());

		mVertexBuffer = graphicEngine.CreateVertexBuffer(mMesh->GetVertices());
		if (mMesh->IsIndexed())
			mIndexBuffer = graphicEngine.CreateIndexBuffer(mMesh->GetIndices());

		for (const Mesh::SubMesh& subMesh : mMesh->GetSubMeshes())
		{
			unsigned int materialIndex = subMesh.MaterialIndex;

			std::shared_ptr<Material> material = materials[materialIndex];

			mMaterials.insert(material);
			mGeometries[material.get()].Add({ subMesh.Count, subMesh.FirstVertex, subMesh.FirstIndex, subMesh.PrimitiveTopology });
		}
	}

	const std::shared_ptr<Mesh>& Shape::GetMesh() const
	{
		return mMesh;
	}

	const std::shared_ptr<VertexBuffer>& Shape::GetVertexBuffer() const
	{
		return mVertexBuffer;
	}

	const std::shared_ptr<IndexBuffer>& Shape::GetIndexBuffer() const
	{
		return mIndexBuffer;
	}

	const std::unordered_set<std::shared_ptr<Material>>& Shape::GetMaterials() const
	{
		return mMaterials;
	}

	const Array<Shape::Geometry>& Shape::GetGeometries(const Material* material) const
	{
		assert(material != nullptr);
		return mGeometries.at(material);
	}

	SceneNode::SceneNode(Scene* scene, std::shared_ptr<TransformationNode> transformationNode) :
		Transformable(std::move(transformationNode)),
		mScene(scene),
		mVisible(true)
	{
		/// Nothing
	}

	void SceneNode::AddShape(std::shared_ptr<Shape> shape)
	{
		std::shared_ptr<VertexArray> vertices = shape->GetVertexBuffer()->GetVertexArray();
		const std::shared_ptr<VertexLayout>& vertexLayout = vertices->GetVertexLayout();

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

				mBoundingBox.Extend(point);
			}
		}

		mShapes.Add(std::move(shape));
	}

	void SceneNode::AddToQueue(RenderQueue& queue, const Camera& camera)
	{
		assert(mVisible);

		Transformation cameraTransformation = camera.GetWorldTransformation();
		const Transformation& worldTransformation = GetTransformationNode()->GetWorldTransformation();
		float distanceToCamera = Vector3<float>::DistanceBetween(worldTransformation.GetTranslation(), cameraTransformation.GetTranslation());

		StaticMeshRecordInfo recordInfo;
		recordInfo.Model = worldTransformation.ToMatrix();

		for (std::shared_ptr<Shape> shape : mShapes)
		{
			std::shared_ptr<VertexBuffer> vertexBuffer = shape->GetVertexBuffer();

			if (std::shared_ptr<IndexBuffer> indexBuffer = shape->GetIndexBuffer())
			{
				recordInfo.IndexBuffer = indexBuffer->GetUnderlyingResource();
				recordInfo.IndexType = indexBuffer->GetIndexArray()->GetIndexType();
				recordInfo.IndexBufferOffset = 0;
			}

			recordInfo.VertexBufferCount = 1;
			recordInfo.VertexBuffers[0].Buffer = vertexBuffer->GetUnderlyingResource();
			recordInfo.VertexBuffers[0].Offset = 0;
			recordInfo.VertexBuffers[0].VertexLayout = vertexBuffer->GetVertexArray()->GetVertexLayout().get();

			for (const std::shared_ptr<Material>& material : shape->GetMaterials())
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
				for (Material::TextureType textureType : Enum<Material::TextureType>::Values)
				{
					if (std::shared_ptr<TextureResource> texture = material->GetTexture(textureType))
						recordInfo.MaterialTextures[textureType] = texture->GetView();
				}

				for (const Shape::Geometry& geometry : shape->GetGeometries(material.get()))
				{
					recordInfo.Count = geometry.Count;
					recordInfo.FirstVertex = geometry.FirstVertex;
					recordInfo.FirstIndex = geometry.FirstIndex;

					queue.PushStaticMesh(recordInfo, distanceToCamera);
				}
			}
		}
	}

	bool SceneNode::IsVisible() const
	{
		return mVisible;
	}

	void SceneNode::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	const Volume<float>& SceneNode::GetBoundingVolume() const
	{
		return mBoundingBox;
	}
}
