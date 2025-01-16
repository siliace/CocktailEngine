#include <Cocktail/Core/System/Concurrency/NullMutex.hpp>

namespace Ck
{
	NullMutex::NullMutex() noexcept = default;

	NullMutex::~NullMutex() noexcept = default;
	
	bool NullMutex::try_lock() noexcept
	{
		return true;
	}

	void NullMutex::lock() noexcept
	{
	}

	void NullMutex::unlock() noexcept
	{
	}
}
