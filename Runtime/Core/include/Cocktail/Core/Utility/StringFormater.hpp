#ifndef COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP
#define COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP

#include <cstdio>
#include <cctype>
#include <cwctype>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/StringView.hpp>

#include "StringUtils.hpp"

namespace Ck
{
    template <typename T, typename Enable = void>
    struct Formater
    {
        T Apply(T value) const noexcept
        {
            return value;
        }
    };

    template <typename E>
    struct Formater<E, std::enable_if_t<std::is_enum_v<E>>>
    {
        const AnsiChar* Apply(E value) const noexcept
        {
            return Enum<E>::ToString(value);
        }
    };

    template <>
    struct Formater<StringView, void>
    {
        const TextChar* Apply(const StringView& value) const noexcept
        {
            return value.GetData();
        }
    };

    template <typename TChar>
    class StringFormater
    {
    public:

        using CharType = TChar;

        template <typename... Args>
        static void FormatTo(Array<CharType>& area, const CharType* format, Args&&... args)
        {
            area.Clear();

            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                std::size_t size = std::sprintf(nullptr, format, Formater<std::decay_t<Args>>().Apply(std::forward<Args>(args))...) + 1;
                if (size <= 0)
                    return;

                area.Resize(size);

                std::sprintf(area.GetData(), format, Formater<std::decay_t<Args>>().Apply(std::forward<Args>(args))...);
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
#ifdef COCKTAIL_OS_WINDOWS
                std::size_t size = std::swprintf(nullptr, 0, format, Formater<std::decay_t<Args>>().Apply(std::forward<Args>(args))...) + 1;
                if (size <= 0)
                    return;

                area.Resize(size);

                std::swprintf(area.GetData(), area.GetSize(), format, Formater<std::decay_t<Args>>().Apply(std::forward<Args>(args))...);
#else
                area.Append(format, StringUtils<CharType>::GetLength(format));
#endif
            }
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP
