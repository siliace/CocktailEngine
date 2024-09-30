#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
	Ref<VertexArray> VertexArray::Merge(const VertexArray& lhs, const VertexArray& rhs)
	{
		const Ref<VertexLayout>& lhsVertexLayout = lhs.GetVertexLayout();
		const Ref<VertexLayout>& rhsVertexLayout = rhs.GetVertexLayout();

		if (lhsVertexLayout->IsInstanced() != rhsVertexLayout->IsInstanced())
			return nullptr;

		if (lhsVertexLayout->IsInstanced() && lhsVertexLayout->GetDivisor() != rhsVertexLayout->GetDivisor())
			return nullptr;

		Ref<VertexLayout> vertexLayout = VertexLayout::New(lhsVertexLayout->IsInstanced(), lhsVertexLayout->GetDivisor());

		return nullptr;
	}

	VertexArray::VertexArray(Ref<VertexLayout> layout, std::size_t vertexCount, bool interlaced) :
		mVertexLayout(std::move(layout)),
		mVertexCount(vertexCount),
		mInterlaced(interlaced)
	{
		mStorage = std::make_unique<unsigned char[]>(mVertexCount * mVertexLayout->GetStride());
	}

	VertexRef VertexArray::At(std::size_t index) const
	{
		if (index >= mVertexCount)
			throw std::out_of_range("VertexArray subscript out of range");

		return VertexRef(mVertexLayout.Get(), mStorage.get(), index, mVertexCount, mInterlaced);
	}

	const Ref<VertexLayout>& VertexArray::GetVertexLayout() const
	{
		return mVertexLayout;
	}

	std::size_t VertexArray::GetVertexCount() const
	{
		return mVertexCount;
	}

	std::size_t VertexArray::GetSize() const
	{
		return mVertexCount * mVertexLayout->GetStride();
	}

	const void* VertexArray::GetPointer() const
	{
		return mStorage.get();
	}
}
