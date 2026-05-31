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

            auto parameters = std::make_tuple(Formatter<std::decay_t<Args>>().Apply(std::forward<Args>(args))...);

            auto ComputeSizeFunction = [&](auto&& formatter) {
                return std::apply(
                    [&](auto const&... p) {
                        return formatter(p...);
                    },
                    parameters);
            };

            auto WriteBufferFunction = [&](auto&& formatter, CharType* buffer) {
                std::apply(
                    [&](auto const&... p) {
                        formatter(buffer, p...);
                    },
                    parameters);
            };

            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                auto size = ComputeSizeFunction([&](auto const&... p) {
                    return std::snprintf(nullptr, 0, format, p...);
                }) + 1;

                if (size == 0)
                    return;

                area.Resize(size);

                WriteBufferFunction([&](CharType* buffer, auto const&... p) {
                    std::snprintf(buffer, area.GetSize(), format, p...);
                }, area.GetData());
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                auto size = ComputeSizeFunction([&](auto const&... p) {
                    return std::swprintf(nullptr, 0, format, p...);
                }) + 1;

                if (size <= 0)
                    return;

                area.Resize(size);

                WriteBufferFunction([&](CharType* buffer, auto const&... p) {
                    std::swprintf(buffer, area.GetSize(), format, p...);
                }, area.GetData());
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                auto size = ComputeSizeFunction([&](auto const&... p) {
                    return std::snprintf(nullptr, 0, reinterpret_cast<const AnsiChar*>(format), p...);
                }) + 1;

                if (size <= 0)
                    return;

                area.Resize(size);

                WriteBufferFunction([&](CharType* buffer, auto const&... p) {
                    std::snprintf(reinterpret_cast<AnsiChar*>(buffer), area.GetSize(), reinterpret_cast<const AnsiChar*>(format), p...);
                }, area.GetData());
            }
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_STRINGFORMATER_HPP
