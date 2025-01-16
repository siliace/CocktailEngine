#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>

namespace Ck
{
	VertexBuffer::VertexBuffer(Ref<GraphicEngine> graphicEngine, Ref<VertexArray> vertices, std::string_view name) :
		Extends<VertexBuffer, BufferResource>(std::move(graphicEngine), Renderer::BufferUsageFlagBits::Vertex, vertices->GetVertices().GetSize(), name),
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
		GetGraphicEngine()->UploadBuffer(Self(), firstVertexOffset, length, mVertices->GetVertices().GetData() + firstVertexOffset);
	}

	Ref<VertexArray> VertexBuffer::GetVertexArray() const
	{
		return mVertices;
	}
}
