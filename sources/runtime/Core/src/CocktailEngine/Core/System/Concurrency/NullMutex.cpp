#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>

namespace Ck
{
	NullMutex::NullMutex() noexcept = default;

	NullMutex::~NullMutex() noexcept = default;

	void NullMutex::Lock() noexcept
	{
	}
	
	bool NullMutex::TryLock() noexcept
	{
		return true;
	}

	void NullMutex::Unlock() noexcept
	{
	}
}
