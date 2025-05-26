#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp>

namespace Ck
{
	IndexBuffer::IndexBuffer(std::shared_ptr<GraphicEngine> graphicEngine, std::shared_ptr<IndexArray> indices, std::string_view name) :
		BufferResource(std::move(graphicEngine), Renderer::BufferUsageFlagBits::Index, indices->GetIndices().GetSize(), name),
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
		GetGraphicEngine()->UploadBuffer(shared_from_this(), firstVertexOffset, length, mIndices->GetIndices().GetData() + firstVertexOffset);
	}

	std::shared_ptr<IndexArray> IndexBuffer::GetIndexArray() const
	{
		return mIndices;
	}
}
