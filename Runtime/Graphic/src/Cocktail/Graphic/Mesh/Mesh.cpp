#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>
#include <Cocktail/Graphic/Mesh/Mesh.hpp>

namespace Ck
{
	Mesh::Mesh(std::shared_ptr<VertexArray> vertices, std::shared_ptr<IndexArray> indices, Array<SubMesh> subMeshes) :
		mVertices(std::move(vertices)),
		mIndices(std::move(indices)),
		mSubMeshes(std::move(subMeshes))
	{
		assert(mVertices);
		assert(!mSubMeshes.IsEmpty());

		for (const SubMesh& subMesh : mSubMeshes)
			mMaterialIndices.insert(subMesh.MaterialIndex);

		GenerateBoundingBox();
	}

	Mesh::Mesh(std::shared_ptr<VertexArray> vertices, std::shared_ptr<IndexArray> indices, Renderer::PrimitiveTopology primitiveTopology) :
		mVertices(std::move(vertices)),
		mIndices(std::move(indices))
	{
		assert(mVertices);

		SubMesh subMesh;
		subMesh.Count = mIndices ? mIndices->GetIndexCount() : mVertices->GetVertexCount();
		subMesh.PrimitiveTopology = primitiveTopology;
		subMesh.MaterialIndex = 0;
		mSubMeshes.Add(subMesh);

		mMaterialIndices.insert(0);

		GenerateBoundingBox();
	}

	bool Mesh::IsIndexed() const
	{
		return mIndices != nullptr;
	}

	const std::shared_ptr<VertexArray>& Mesh::GetVertices() const
	{
		return mVertices;
	}

	const std::shared_ptr<IndexArray>& Mesh::GetIndices() const
	{
		return mIndices;
	}

	const Array<Mesh::SubMesh>& Mesh::GetSubMeshes() const
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
		std::shared_ptr<VertexLayout> vertexLayout = mVertices->GetVertexLayout();
		const VertexAttribute* positionAttribute = vertexLayout->FindAttribute(VertexAttributeSemantic::Position);
		if (!positionAttribute)
			return;

		assert(positionAttribute->GetType() == DataType::Float32);
		assert(positionAttribute->GetElementCount() == 2 || positionAttribute->GetElementCount() == 3);

		Vector3<float> maxBounds = Vector3<float>(std::numeric_limits<float>::lowest());
		Vector3<float> minBounds = Vector3<float>(std::numeric_limits<float>::max());

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
