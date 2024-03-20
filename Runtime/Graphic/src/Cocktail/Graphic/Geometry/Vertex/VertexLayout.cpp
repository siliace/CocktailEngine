#include <cassert>

#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>

namespace Ck
{
	Ref<VertexLayout> VertexLayout::Merge(const VertexLayout& lhs, const VertexLayout& rhs)
	{
		assert(lhs.GetDivisor() == rhs.GetDivisor());
		assert(lhs.IsInstanced() == rhs.IsInstanced());

		Ref<VertexLayout> merged = VertexLayout::New(lhs.IsInstanced(), lhs.GetDivisor());

		for (const VertexAttribute& lhsAttribute : lhs.GetAttributes())
		{
			const VertexAttribute* rhsAttribute = rhs.FindAttribute(lhsAttribute.GetSemantic());
			if (rhsAttribute)
			{
				assert(lhsAttribute.GetType() == rhsAttribute->GetType());
				assert(lhsAttribute.GetElementCount() == rhsAttribute->GetElementCount());
			}

			merged->CreateAttribute(lhsAttribute.GetSemantic(), lhsAttribute.GetType(), lhsAttribute.GetElementCount());
		}

		for (const VertexAttribute& rhsAttribute : rhs.GetAttributes())
		{
			const VertexAttribute* lhsAttribute = lhs.FindAttribute(rhsAttribute.GetSemantic());
			if (lhsAttribute)
				continue;

			merged->CreateAttribute(rhsAttribute.GetSemantic(), rhsAttribute.GetType(), rhsAttribute.GetElementCount());
		}

		return merged;
	}

	VertexLayout::VertexLayout(bool instanced, unsigned int divisor) :
		mInstanced(instanced),
		mDivisor(divisor)
	{
		/// Nothing
	}

	const VertexAttribute& VertexLayout::CreateAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength, bool normalized)
	{
		assert(FindAttribute(semantic) == nullptr);

		VertexAttribute& attribute = mAttributes.emplace_back(
			VertexAttribute(semantic, dataType, GetStride(), elementCount, arrayLength, normalized)
		);
		mStride.SetDirty();
		mUsageMask.SetDirty();

		return attribute;
	}

	const VertexAttribute* VertexLayout::FindAttribute(VertexAttributeSemantic semantic) const
	{
		for (const VertexAttribute& attribute : mAttributes)
		{
			if (attribute.GetSemantic() == semantic)
				return &attribute;
		}

		return nullptr;
	}

	unsigned int VertexLayout::GetStride() const
	{
		return mStride.Get([&]() {
			unsigned int stride = 0;
			for (const VertexAttribute& attribute : mAttributes)
				stride += attribute.GetStride();

			return stride;
		});
	}

	Flags<VertexAttributeSemantic> VertexLayout::GetUsageMask() const
	{
		return mUsageMask.Get([&]() {
			Flags<VertexAttributeSemantic> usageMask;
			for (const VertexAttribute& attribute : mAttributes)
				usageMask |= attribute.GetSemantic();

			return usageMask;
		});
	}

	bool VertexLayout::IsInstanced() const
	{
		return mInstanced;
	}

	unsigned int VertexLayout::GetDivisor() const
	{
		return mDivisor;
	}

	const std::vector<VertexAttribute>& VertexLayout::GetAttributes() const
	{
		return mAttributes;
	}
}
