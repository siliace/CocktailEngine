#include <Cocktail/Graphic/Mesh/Mesh.hpp>

namespace Ck
{
	Mesh::Mesh(Ref<GraphicEngine> graphicEngine, Ref<VertexArray> vertices, Ref<IndexArray> indices, Renderer::PrimitiveTopology primitiveTopology) :
		mGraphicEngine(std::move(graphicEngine)),
		mPrimitiveTopology(primitiveTopology)
	{
		assert(vertices && vertices->GetVertexCount() > 0);
		mVertexBuffer = mGraphicEngine->CreateVertexBuffer(std::move(vertices));

		if (indices && indices->GetIndexCount() > 0)
			mIndexBuffer = mGraphicEngine->CreateIndexBuffer(std::move(indices));
	}

	Ref<VertexBuffer> Mesh::GetVertexBuffer() const
	{
		return mVertexBuffer;
	}

	Ref<IndexBuffer> Mesh::GetIndexBuffer() const
	{
		return mIndexBuffer;
	}

	Renderer::PrimitiveTopology Mesh::GetPrimitiveTopology() const
	{
		return mPrimitiveTopology;
	}
}
