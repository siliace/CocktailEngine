#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/VertexBuffer.hpp>

namespace Ck
{
	VertexBuffer::VertexBuffer(Ref<GraphicEngine> graphicEngine, Ref<Renderer::Buffer> buffer, Ref<VertexArray> vertices) :
		mGraphicEngine(std::move(graphicEngine)),
		mBuffer(std::move(buffer)),
		mVertices(std::move(vertices))
	{
		/// Nothing
	}

	void VertexBuffer::Upload()
	{
		mGraphicEngine->UploadBuffer(Self(), 0, mVertices->GetSize(), mVertices->GetPointer());
	}

	Ref<GraphicEngine> VertexBuffer::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}

	Renderer::Buffer* VertexBuffer::GetUnderlyingResource() const
	{
		return mBuffer.Get();
	}

	Ref<VertexArray> VertexBuffer::GetVertexArray() const
	{
		return mVertices;
	}
}
