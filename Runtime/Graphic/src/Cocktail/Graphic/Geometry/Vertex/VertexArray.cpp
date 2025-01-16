#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
	VertexArray::VertexArray(Ref<VertexLayout> layout, std::size_t vertexCount, bool interlaced) :
		mVertexLayout(std::move(layout)),
		mVertexCount(vertexCount),
		mInterlaced(interlaced)
	{
		mVertices.Resize(mVertexCount * mVertexLayout->GetStride());
	}

	void VertexArray::Merge(const VertexArray& other)
	{
		assert(mVertexLayout->IsCompatibleWith(*other.GetVertexLayout()));

		if (*mVertexLayout != *other.GetVertexLayout())
		{
			std::size_t previousVertexCount = mVertexCount;

			mVertexCount += other.GetVertexCount();
			mVertices.Resize(mVertexCount * mVertexLayout->GetStride());

			// Even if layouts are compatibles, data ordering might change so we cannot do a raw copy
			for (std::size_t i = 0; i < other.GetVertexCount(); i++)
			{
				VertexRef vertexRef = At(previousVertexCount + i);
				VertexConstRef otherVertexRef = other.At(i);

				for (const VertexAttribute& vertexAttribute : mVertexLayout->GetAttributes())
				{
					const void* otherAttributeValue = otherVertexRef.Get(vertexAttribute.GetSemantic());
					vertexRef.Set(vertexAttribute.GetSemantic(), otherAttributeValue, vertexAttribute.GetType(), vertexAttribute.GetElementCount() * vertexAttribute.GetArrayLength());
				}
			}
		}
		else
		{
			mVertexCount += other.GetVertexCount();
			mVertices.Append(other.GetVertices());
		}
	}

	VertexRef VertexArray::At(std::size_t index)
	{
		assert(index < mVertexCount);
		return VertexRef(mVertexLayout.Get(), &mVertices, index, mVertexCount, mInterlaced);
	}

	VertexConstRef VertexArray::At(std::size_t index) const
	{
		assert(index < mVertexCount);
		return VertexConstRef(mVertexLayout.Get(), &mVertices, index, mVertexCount, mInterlaced);
	}

	const Ref<VertexLayout>& VertexArray::GetVertexLayout() const
	{
		return mVertexLayout;
	}

	std::size_t VertexArray::GetVertexCount() const
	{
		return mVertexCount;
	}

	const ByteArray& VertexArray::GetVertices() const
	{
		return mVertices;
	}
}
