#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	Exception::Exception():
		mNested(nullptr)
	{
		/// Nothing
	}

	Exception::Exception(String message):
		mMessage(std::move(message)),
		mNested(nullptr)
	{
		/// Nothing
	}

	Exception::Exception(const Exception& nested, String message):
		mMessage(std::move(message))
	{
		mNested = nested.Clone();
	}

	Exception::Exception(const Exception& other):
		std::exception(other),
		mMessage(other.mMessage)
	{
		mNested = other.mNested ? other.mNested->Clone() : nullptr;
	}

	Exception::Exception(Exception&& other) noexcept:
		std::exception(other),
		mMessage(std::move(other.mMessage)),
		mNested(std::exchange(other.mNested, nullptr))
	{
		/// Nothing
	}

	Exception::~Exception()
	{
		delete mNested;
	}

	Exception& Exception::operator=(const Exception& other)
	{
		if (this == &other)
			return *this;

		std::exception::operator=(other);
		mMessage = other.mMessage;
		mNested = other.mNested;

		return *this;
	}

	Exception& Exception::operator=(Exception&& other) noexcept
	{
		if (this == &other)
			return *this;

		std::exception::operator=(other);
		mMessage = std::move(other.mMessage);
		mNested = std::exchange(other.mNested, nullptr);

		return *this;
	}

	char const* Exception::what() const noexcept
	{
		return CK_TEXT_TO_ANSI(GetName().GetData());
	}

	StringView Exception::GetName() const
	{
		return CK_TEXT("Exception");
	}

	const String& Exception::GetMessage() const
	{
		return mMessage;
	}

	Exception* Exception::Clone() const
	{
		return new Exception(*this);
	}
}
