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
}
