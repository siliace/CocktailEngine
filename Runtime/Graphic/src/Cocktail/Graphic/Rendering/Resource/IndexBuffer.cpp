#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp>

namespace Ck
{
	IndexBuffer::IndexBuffer(Ref<GraphicEngine> graphicEngine, Ref<IndexArray> indices, std::string_view name) :
		Extends<Ck::IndexBuffer, Ck::BufferResource>(std::move(graphicEngine), Renderer::BufferUsageFlagBits::Index, indices->GetIndices().GetSize(), name),
		mIndices(std::move(indices))
	{
		/// Nothing
	}

	void IndexBuffer::Upload()
	{
		Upload(0, mIndices->GetIndexCount());
	}

	void IndexBuffer::Upload(std::size_t firstIndex, std::size_t indexCount)
	{
		const unsigned int indexStride = Renderer::ToDataType(mIndices->GetIndexType()).GetSize();
		std::size_t firstVertexOffset = firstIndex * indexStride;
		std::size_t length = indexCount * indexStride;
		GetGraphicEngine()->UploadBuffer(Self(), firstVertexOffset, length, mIndices->GetIndices().GetData() + firstVertexOffset);
	}

	Ref<IndexArray> IndexBuffer::GetIndexArray() const
	{
		return mIndices;
	}
}
