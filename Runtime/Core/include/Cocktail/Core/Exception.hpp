#ifndef COCKTAIL_CORE_EXCEPTION_HPP
#define COCKTAIL_CORE_EXCEPTION_HPP

#include <stdexcept>

#include <fmt/core.h>

#define COCKTAIL_DECLARE_EXCEPTION(__Name, __Message) \
	class __Name : public std::runtime_error \
	{ \
	public: \
		static constexpr const char* Message = __Message; \
		template <typename... Args> \
		static std::string FormatExceptionMessage(std::string_view format, Args&&... args) \
		{ \
			return fmt::format("{} : {}", Message, fmt::format(format, std::forward<Args>(args)...)); \
		} \
		__Name() : \
			std::runtime_error(Message) \
		{} \
		template <typename... Args> \
		explicit __Name(std::string_view format, Args&&... args) : \
			std::runtime_error(FormatExceptionMessage(format, std::forward<Args>(args)...)) \
		{} \
	} \

#endif // COCKTAIL_CORE_EXCEPTION_HPP
