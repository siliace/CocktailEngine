#include <Cocktail/Core/Color.hpp>

namespace Ck
{
	const Color Black = {0, 0, 0};
	const Color White = {255, 255, 255};
	const Color Red = {255, 0, 0};
	const Color Green = {0, 255, 0};
	const Color Blue = {0, 0, 255};
	const Color Transparent = {0, 0, 0, 255};

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
		return { R / 255.f, G / 255.f, B / 255.f, A / 255.f };
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

	bool LinearColor::operator==(const LinearColor& rhs) const
	{
		return std::tie(R, G, B, A) == std::tie(rhs.R, rhs.G, rhs.B, rhs.A);
	}

	bool LinearColor::operator!=(const LinearColor& rhs) const
	{
		return !(*this == rhs);
	}
}
