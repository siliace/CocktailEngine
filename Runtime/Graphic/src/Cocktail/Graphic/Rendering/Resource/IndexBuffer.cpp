#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/IndexBuffer.hpp>

namespace Ck
{
	IndexBuffer::IndexBuffer(Ref<GraphicEngine> graphicEngine, Ref<Renderer::Buffer> buffer, Ref<IndexArray> indices) :
		mGraphicEngine(std::move(graphicEngine)),
		mBuffer(std::move(buffer)),
		mIndices(std::move(indices))
	{
		/// Nothing
	}

	void IndexBuffer::Upload()
	{
		mGraphicEngine->UploadBuffer(Self(), 0, mIndices->GetSize(), mIndices->GetPointer());
	}

	Ref<GraphicEngine> IndexBuffer::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}

	Renderer::Buffer* IndexBuffer::GetUnderlyingResource() const
	{
		return mBuffer.Get();
	}

	Ref<IndexArray> IndexBuffer::GetIndexArray() const
	{
		return mIndices;
	}
}
