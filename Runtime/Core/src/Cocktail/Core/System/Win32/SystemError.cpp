#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck
{
	class Win32ErrorCategory : public std::error_category
	{
		const char* name() const noexcept override
		{
			return "Win32ErrorCategory";
		}

		std::string message(int errorCode) const override
		{
			LPSTR messageBuffer;

			if (errorCode == 0)
				return "";

			DWORD size = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				errorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&messageBuffer), // Send a pointer of string so WinAPI can allocate it accordingly to the message length
				0,
				nullptr
			);

			std::string error;
			error.assign(messageBuffer, size);

			LocalFree(messageBuffer);

			return error;
		}
	};

	std::system_error SystemError::GetLastError()
	{
		return std::system_error(static_cast<int>(::GetLastError()), GetSystemErrorCategory());
	}

	const std::error_category& SystemError::GetSystemErrorCategory()
	{
		static Win32ErrorCategory errorCategory;
		return errorCategory;
	}
}
