#ifndef COCKTAIL_CORE_EXCEPTION_HPP
#define COCKTAIL_CORE_EXCEPTION_HPP

#include <stdexcept>

#include <fmt/core.h>

#define COCKTAIL_DECLARE_EXCEPTION_BASE(__Name, __Message, __Base) \
	class __Name : public __Base \
	{ \
	public: \
		static constexpr const char* Message = __Message; \
		template <typename... Args> \
		static std::string FormatExceptionMessage(std::string_view format, Args&&... args) \
		{ \
			return fmt::format("{} : {}", Message, fmt::format(format, std::forward<Args>(args)...)); \
		} \
		__Name() : \
			__Base(Message) \
		{} \
		template <typename... Args> \
		explicit __Name(std::string_view format, Args&&... args) : \
			__Base(FormatExceptionMessage(format, std::forward<Args>(args)...)) \
		{} \
	} \

#define COCKTAIL_DECLARE_EXCEPTION(__Name, __Message) COCKTAIL_DECLARE_EXCEPTION_BASE(__Name, __Message, std::exception)

#endif // COCKTAIL_CORE_EXCEPTION_HPP
