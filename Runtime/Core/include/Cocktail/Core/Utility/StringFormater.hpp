#ifndef COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP
#define COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP

#include <cstdio>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Enum.hpp>

namespace Ck
{
    template <typename T, typename Enabler = void>
    struct Formatter
    {
        T Apply(T value) const noexcept
        {
            return value;
        }
    };

    template <typename E>
    struct Formatter<E, std::enable_if_t<std::is_enum_v<E>>>
    {
        const AnsiChar* Apply(E value) const noexcept
        {
            return Enum<E>::ToString(value);
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
                std::size_t size = std::snprintf(nullptr, 0, format, Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...) + 1;
                if (size <= 0)
                    return;

                area.Resize(size);

                std::snprintf(area.GetData(), area.GetSize(), format, Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...);
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                std::size_t size = std::swprintf(nullptr, 0, format, Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...) + 1;
                if (size <= 0)
                    return;

                area.Resize(size);

                std::swprintf(area.GetData(), area.GetSize(), format, Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...);
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                std::size_t size = std::snprintf(nullptr, 0, reinterpret_cast<const char*>(format), Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...) + 1;
                if (size <= 0)
                    return;

                area.Resize(size);

                std::snprintf(reinterpret_cast<char*>(area.GetData()), area.GetSize(), reinterpret_cast<const char*>(format), Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...);
            }
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP
