#ifndef COCKTAILENGINE_CORE_UTILITY_STRINGFORMATER_HPP
#define COCKTAILENGINE_CORE_UTILITY_STRINGFORMATER_HPP

#include <cstdio>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Enum.hpp>

namespace Ck
{
    /**
     * \brief Get a valid, null terminated, empty string for the given character type
     *
     * Passing a null pointer to a `%s` directive is undefined behaviour, so the Formatter
     * specializations must still hand out a dereferenceable pointer when the string they
     * adapt is empty.
     *
     * \tparam TChar The character type of the returned string
     *
     * \return A pointer to a null terminated empty string
     */
    template <typename TChar>
    const TChar* GetEmptyFormatArgument() noexcept
    {
        static constexpr TChar empty[1] = {};
        return empty;
    }

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
            const AnsiChar* data = Enum<E>::ToString(value);
            return data != nullptr ? data : GetEmptyFormatArgument<AnsiChar>();
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

            auto parameters = std::make_tuple(Formatter<std::decay_t<Args>>().Apply(Forward<Args>(args))...);

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

#endif // COCKTAILENGINE_CORE_UTILITY_STRINGFORMATER_HPP
