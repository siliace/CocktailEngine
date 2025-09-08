#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>

namespace Ck
{
	VertexBuffer::VertexBuffer(std::shared_ptr<GraphicEngine> graphicEngine, std::shared_ptr<VertexArray> vertices, const AnsiChar* name) :
		BufferResource(std::move(graphicEngine), Renderer::BufferUsageFlagBits::Vertex, vertices->GetVertices().GetSize(), name),
		mVertices(std::move(vertices))
	{
		/// Nothing
	}

	void VertexBuffer::Upload()
	{
		Upload(0, mVertices->GetVertexCount());
	}

	void VertexBuffer::Upload(std::size_t firstVertex, std::size_t vertexCount)
	{
		const unsigned int vertexStride = mVertices->GetVertexLayout()->GetStride();
		std::size_t firstVertexOffset = firstVertex * vertexStride;
		std::size_t length = vertexCount * vertexStride;
		GetGraphicEngine()->UploadBuffer(shared_from_this(), firstVertexOffset, length, mVertices->GetVertices().GetData() + firstVertexOffset);
	}

	std::shared_ptr<VertexArray> VertexBuffer::GetVertexArray() const
	{
		return mVertices;
	}
}
