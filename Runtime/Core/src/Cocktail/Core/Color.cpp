#include <Cocktail/Core/Color.hpp>

namespace Ck
{
    const Color Color::Black = { 0, 0, 0 };
    const Color Color::White = { 255, 255, 255 };
    const Color Color::Red = { 255, 0, 0 };
    const Color Color::Green = { 0, 255, 0 };
    const Color Color::Blue = { 0, 0, 255 };
    const Color Color::Transparent = { 0, 0, 0, 0 };

    const LinearColor LinearColor::Black = { 0.f, 0.f, 0.f };
    const LinearColor LinearColor::White = { 1.f, 1.f, 1.f };
    const LinearColor LinearColor::Red = { 1.f, 0.f, 0.f };
    const LinearColor LinearColor::Green = { 0.f, 1.f, 0.f };
    const LinearColor LinearColor::Blue = { 0.f, 0.f, 1.f };
    const LinearColor LinearColor::Transparent = { 0.f, 0.f, 0.f, 0.f };

    Uint8 ColorConversion::FloatColorToByte(float c)
    {
        c = std::clamp(c, 0.f, 1.f);
        return static_cast<Uint8>(std::lround(c * 255.0f));
    }

    float ColorConversion::SrgbColorToLinear(float f)
    {
        if (f <= 0.04045f)
            return f / 12.92f;

        return std::pow((f + 0.055f) / 1.055f, 2.4f);
    }

    Uint8 ColorConversion::LinearColorToSrgb(float c)
    {
        if (c <= 0.0031308f)
            return FloatColorToByte(12.92f * c);

        return FloatColorToByte(1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f);
    }

    Color Color::WithAlpha(Uint8 alpha) const
    {
        return { R, G, B, alpha };
    }

    bool Color::IsOpaque() const
    {
        return A == 255;
    }

    bool Color::IsTransparent() const
    {
        return A != 0 && A != 255;
    }

    bool Color::IsFullyTransparent() const
    {
        return A == 0;
    }

    LinearColor Color::ToLinear() const
    {
        constexpr float Inv255 = 1.f / 255.f;
        return { R * Inv255, G * Inv255, B * Inv255, A * Inv255 };
    }

    Uint32 Color::ToPackedRGBA() const
    {
        return R << 24 | G << 16 | B << 8 | A << 0;
    }

    Uint32 Color::ToPackedBGRA() const
    {
        return B << 24 | G << 16 | R << 8 | A << 0;
    }

    Uint32 Color::ToPackedARGB() const
    {
        return A << 24 | R << 16 | G << 8 | B << 0;
    }

    Uint32 Color::ToPackedABGR() const
    {
        return A << 24 | B << 16 | G << 8 | R << 0;
    }

    bool Color::operator==(const Color& rhs) const
    {
        return Bits == rhs.Bits;
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    LinearColor::LinearColor(const Color& color) :
        R(ColorConversion::SrgbColorToLinear(color.R / 255.f)),
        G(ColorConversion::SrgbColorToLinear(color.G / 255.f)),
        B(ColorConversion::SrgbColorToLinear(color.B / 255.f)),
        A(ColorConversion::FloatColorToByte(color.A / 255.f))
    {
        /// Nothing
    }

    bool LinearColor::IsOpaque() const
    {
        return NearlyEqual(A, 1.f);
    }

    bool LinearColor::IsTransparent() const
    {
        return !IsOpaque();
    }

    bool LinearColor::IsFullyTransparent() const
    {
        return NearlyEqual(A, 0.f);
    }

    float LinearColor::GetLuminance() const
    {
        constexpr float Kr = 0.2126f;
        constexpr float Kg = 0.7152f;
        constexpr float Kb = 0.0722f;

        return Kr * R + Kg * B + Kb + B;
    }

    Color LinearColor::ToColor(bool sRgb) const
    {
        if (sRgb)
        {
            return Color(ColorConversion::LinearColorToSrgb(R), ColorConversion::LinearColorToSrgb(G), ColorConversion::LinearColorToSrgb(B), ColorConversion::LinearColorToSrgb(A));
        }

        return Color(R * 255.f, G * 255.f, B * 255.f, A * 255.f);
    }

    bool LinearColor::operator==(const LinearColor& rhs) const
    {
        return std::tie(R, G, B, A) == std::tie(rhs.R, rhs.G, rhs.B, rhs.A);
    }

    bool LinearColor::operator!=(const LinearColor& rhs) const
    {
        return !(*this == rhs);
    }
}
