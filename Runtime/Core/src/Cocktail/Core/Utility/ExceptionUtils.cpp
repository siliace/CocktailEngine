#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/ExceptionUtils.hpp>

namespace Ck
{
	void ExceptionUtils::ThrowEmptyOptional()
	{
		throw EmptyOptionalException();
	}

	void ExceptionUtils::ThrowEmptyOptional(const TextChar* message)
	{
		throw EmptyOptionalException(message);
	}

	void ExceptionUtils::ThrowEmptyContainer()
	{
		throw ContainerEmpty();
	}

	void ExceptionUtils::ThrowEmptyContainer(const TextChar* message)
	{
		throw ContainerEmpty(message);
	}

	void ExceptionUtils::ThrowOutOfRange(Uint32 index, Uint32 max)
	{
		throw ContainerOutOfRange(CK_TEXT("Index {} is out of container size {}"), index, max);
	}

	void ExceptionUtils::ThrowOutOfRange(Uint64 index, Uint64 max)
	{
		throw ContainerOutOfRange(CK_TEXT("Index {} is out of container size {}"), index, max);
	}
}
