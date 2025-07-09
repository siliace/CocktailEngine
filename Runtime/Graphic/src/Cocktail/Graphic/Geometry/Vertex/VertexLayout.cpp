#include <cassert>

#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>

namespace Ck
{
	VertexLayout::Builder& VertexLayout::Builder::AddAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength, bool normalized)
	{
		mCreateInfo.VertexAttributes[mCreateInfo.VertexAttributeCount] = {
			semantic, dataType, elementCount, arrayLength, normalized
		};
		++mCreateInfo.VertexAttributeCount;
		return *this;
	}

	VertexLayout::Builder& VertexLayout::Builder::SetInstanced(bool instanced)
	{
		mCreateInfo.Instanced = instanced;
		return *this;
	}

	VertexLayout::Builder& VertexLayout::Builder::SetDivisor(unsigned int divisor)
	{
		mCreateInfo.Divisor = divisor;
		return *this;
	}

	std::shared_ptr<VertexLayout> VertexLayout::Builder::Get()
	{
		return std::make_shared<VertexLayout>(mCreateInfo);
	}

	const VertexLayoutCreateInfo& VertexLayout::Builder::ToCreateInfo() const
	{
		return mCreateInfo;
	}

	VertexLayout::VertexLayout(const VertexLayoutCreateInfo& createInfo) :
		mInstanced(createInfo.Instanced),
		mDivisor(createInfo.Divisor),
		mStride(0)
	{
		assert(!mInstanced || mDivisor > 0);

		for (unsigned int i = 0; i < createInfo.VertexAttributeCount; i++)
		{
			CreateAttribute(
				createInfo.VertexAttributes[i].Semantic,
				createInfo.VertexAttributes[i].DataType,
				createInfo.VertexAttributes[i].ElementCount,
				createInfo.VertexAttributes[i].ArrayLength,
				createInfo.VertexAttributes[i].Normalized
			);
		}
	}

	const VertexAttribute& VertexLayout::CreateAttribute(VertexAttributeSemantic semantic, const DataType& dataType, unsigned int elementCount, unsigned int arrayLength, bool normalized)
	{
		assert(!(mUsageMask & semantic));

		VertexAttribute& attribute = mAttributes.Emplace(
			VertexAttribute(semantic, dataType, GetStride(), elementCount, arrayLength, normalized)
		);

		mUsageMask |= semantic;
		mStride += attribute.GetStride();

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

	bool VertexLayout::IsCompatibleWith(const VertexLayout& other) const
	{
		for (const VertexAttribute& attribute : mAttributes)
		{
			const VertexAttribute* otherAttribute = other.FindAttribute(attribute.GetSemantic());
			if (!otherAttribute || !attribute.IsCompatibleWith(*otherAttribute))
				return false;
		}

		return true;
	}

	unsigned int VertexLayout::GetStride() const
	{
		return mStride;
	}

	Flags<VertexAttributeSemantic> VertexLayout::GetUsageMask() const
	{
		return mUsageMask;
	}

	bool VertexLayout::IsInstanced() const
	{
		return mInstanced;
	}

	unsigned int VertexLayout::GetDivisor() const
	{
		return mDivisor;
	}

	const Array<VertexAttribute>& VertexLayout::GetAttributes() const
	{
		return mAttributes;
	}

	bool VertexLayout::operator==(const VertexLayout& rhs) const
	{
		return std::tie(mInstanced, mDivisor, mStride, mUsageMask, mAttributes) == std::tie(
			rhs.mInstanced, rhs.mDivisor, rhs.mStride, rhs.mUsageMask, rhs.mAttributes);
	}

	bool VertexLayout::operator!=(const VertexLayout& rhs) const
	{
		return !(*this == rhs);
	}
}
