#include <Cocktail/Core/System/Clipboard/Clipboard.hpp>

namespace Ck
{
	std::size_t Clipboard::Get(void* data, std::size_t length, std::size_t offset)
	{
		return ResolveFacadeInstance()->Get(data, length, offset);
	}

	void Clipboard::Set(const void* data, std::size_t length)
	{
		ResolveFacadeInstance()->Set(data, length);
	}

	void Clipboard::Clear()
	{
		ResolveFacadeInstance()->Clear();
	}
}
