#include <Cocktail/Core/Utility/Any.hpp>

namespace Ck
{
	bool Any::IsEmpty() const
	{
		return mHolder == nullptr;
	}

	void Any::Reset()
	{
		mHolder = nullptr;
	}

	void Any::Swap(Any& other)
	{
		std::swap(mHolder, other.mHolder);
	}
}