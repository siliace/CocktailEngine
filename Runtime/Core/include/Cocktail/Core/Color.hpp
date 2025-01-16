#ifndef COCKTAIL_CORE_COLOR_HPP
#define COCKTAIL_CORE_COLOR_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	class LinearColor;

	class COCKTAIL_CORE_API Color
	{
	public:

		union 
		{
			struct { Uint8 R, G, B, A; };
			Uint32 Bits;
		};

		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Transparent;

		/**
		 * \brief 
		 */
		constexpr Color() :
			Bits(0)
		{
			/// Nothing
		}

		explicit Color(Uint32 bits) :
			Bits(bits)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * Create a color from its channels
		 * \param r The value of the red channel
		 * \param g The value of the green channel
		 * \param b The value of the blue channel
		 * \param a The value of the alpha channel
		 */
		constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) :
			R(r),
			G(g),
			B(b),
			A(a)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param alpha 
		 * \return 
		 */
		Color WithAlpha(Uint8 alpha) const;

		/**
		 * \brief Tell whether the color is opaque (i.e no transparency)
		 * \return True if opaque, false otherwise
		 */
		bool IsOpaque() const;

		/**
		 * \brief Tell whether the color has transparency
		 * \return True if transparent, false otherwise
		 */
		bool IsTransparent() const;

		/**
		 * \brief Tell whether the color is fully transparent (i.e no opacity)
		 * \return True if fully transparent, false otherwise
		 */
		bool IsFullyTransparent() const;

		/**
		 * \brief 
		 * \return 
		 */
		LinearColor ToLinear() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint32 ToPackedRGBA() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint32 ToPackedBGRA() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint32 ToPackedARGB() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint32 ToPackedABGR() const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator==(const Color& rhs) const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator!=(const Color& rhs) const;
	};

	class COCKTAIL_CORE_API LinearColor
	{
	public:

		/**
		 * \brief
		 */
		constexpr LinearColor() :
			R(0.f),
			G(0.f),
			B(0.f),
			A(0.f)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param r
		 * \param g
		 * \param b
		 * \param a
		 */
		constexpr LinearColor(float r, float g, float b, float a = 1.f) :
			R(r),
			G(g),
			B(b),
			A(a)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator==(const LinearColor& rhs) const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator!=(const LinearColor& rhs) const;

		float R, G, B, A;
	};
}

#endif // COCKTAIL_CORE_COLOR_HPP
