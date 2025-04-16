#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>
#include <Cocktail/Graphic/Mesh/Mesh.hpp>

namespace Ck
{
	Mesh::Mesh(Ref<VertexArray> vertices, Ref<IndexArray> indices, std::vector<SubMesh> subMeshes) :
		mVertices(std::move(vertices)),
		mIndices(std::move(indices)),
		mSubMeshes(std::move(subMeshes))
	{
		assert(mVertices);
		assert(!mSubMeshes.empty());

		for (const SubMesh& subMesh : mSubMeshes)
			mMaterialIndices.insert(subMesh.MaterialIndex);

		GenerateBoundingBox();
	}

	Mesh::Mesh(Ref<VertexArray> vertices, Ref<IndexArray> indices, Renderer::PrimitiveTopology primitiveTopology) :
		mVertices(std::move(vertices)),
		mIndices(std::move(indices))
	{
		assert(mVertices);

		SubMesh subMesh;
		subMesh.Count = mIndices ? mIndices->GetIndexCount() : mVertices->GetVertexCount();
		subMesh.PrimitiveTopology = primitiveTopology;
		subMesh.MaterialIndex = 0;
		mSubMeshes.push_back(subMesh);

		mMaterialIndices.insert(0);

		GenerateBoundingBox();
	}

	bool Mesh::IsIndexed() const
	{
		return mIndices != nullptr;
	}

	const Ref<VertexArray>& Mesh::GetVertices() const
	{
		return mVertices;
	}

	const Ref<IndexArray>& Mesh::GetIndices() const
	{
		return mIndices;
	}

	const std::vector<Mesh::SubMesh>& Mesh::GetSubMeshes() const
	{
		return mSubMeshes;
	}

	const std::unordered_set<unsigned int>& Mesh::GetMaterialIndices() const
	{
		return mMaterialIndices;
	}

	Box<float> Mesh::GetBoundingBox() const
	{
		return mBoundingBox;
	}

	void Mesh::GenerateBoundingBox()
	{
		Ref<VertexLayout> vertexLayout = mVertices->GetVertexLayout();
		const VertexAttribute* positionAttribute = vertexLayout->FindAttribute(VertexAttributeSemantic::Position);
		if (!positionAttribute)
			return;

		assert(positionAttribute->GetType() == DataType::Float32);
		assert(positionAttribute->GetElementCount() == 2 || positionAttribute->GetElementCount() == 3);

		Vector3<float> maxBounds = Vector3<float>::Zero();
		Vector3<float> minBounds = Vector3<float>::Infinite();

		const bool isFlat = positionAttribute->GetElementCount() == 2;
		for (std::size_t i = 0; i < mVertices->GetVertexCount(); i++)
		{
			VertexRef vertex = mVertices->At(i);

			const float* position = static_cast<const float*>(vertex.Get(VertexAttributeSemantic::Position));

			maxBounds.X() = std::max(maxBounds.X(), position[0]);
			minBounds.X() = std::min(minBounds.X(), position[0]);

			maxBounds.Y() = std::max(maxBounds.Y(), position[1]);
			minBounds.Y() = std::min(minBounds.Y(), position[1]);

			if (!isFlat)
			{
				maxBounds.Z() = std::max(maxBounds.Z(), position[2]);
				minBounds.Z() = std::min(minBounds.Z(), position[2]);
			}
			else
			{
				maxBounds.Z() = 0;
				minBounds.Z() = 0;
			}
		}

		mBoundingBox = Box<float>(minBounds, maxBounds);
	}
}
