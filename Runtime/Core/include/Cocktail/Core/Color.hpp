#ifndef COCKTAIL_CORE_COLOR_HPP
#define COCKTAIL_CORE_COLOR_HPP

#include <cmath>

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    class Color;
    class LinearColor;

    /**
     * \brief Utility class for color conversion operations
     *
     * Provides static functions to convert colors between different representations,
     * including linear space, sRGB space, and 8-bit integer formats.
     *
     * This class is purely static and does not store any state.
     */
    class COCKTAIL_CORE_API ColorConversion
    {
    public:

        /**
         * \brief Converts a normalized float color to an 8-bit integer
         *
         * Clamps the input float to the range [0, 1] and converts it to a
         * corresponding 8-bit value (0-255) using rounding.
         *
         * \param c Input color as a float in [0, 1]
         * \return 8-bit representation of the color
         */
        static Uint8 FloatColorToByte(float c);

        /**
         * \brief Converts a color from sRGB space to linear space
         *
         * Applies the standard sRGB to linear transformation. This is typically
         * used to prepare sRGB color values for physically-based rendering
         * or other calculations in linear space.
         *
         * \param f Input sRGB color component (normalized float in [0, 1])
         * \return Corresponding linear color component
         */
        static float SrgbColorToLinear(float f);

        /**
         * \brief Converts a linear color component to sRGB and returns as 8-bit
         *
         * Applies the standard linear-to-sRGB transformation and converts the
         * result to an 8-bit integer using \ref FloatColorToByte.
         *
         * \param c Input linear color component (normalized float in [0, 1])
         * \return Corresponding sRGB color component as 8-bit integer
         */
        static Uint8 LinearColorToSrgb(float c);
    };

    /**
     * \brief Represents an 8-bit per channel RGBA color
     *
     * Color stores color components using 8-bit unsigned normalized integers.
     * It is typically used for UI, LDR images, serialization and packed color
     * representations.
     *
     * The internal layout allows both per-channel access (R, G, B, A) and
     * packed 32-bit access through \ref Bits.
     */
    class COCKTAIL_CORE_API Color
    {
    public:

        union
        {
            struct
            {
                Uint8 R, G, B, A; /*!< Red, Green, Blue and Alpha components */
            };
            Uint32 Bits; /*!< Packed 32-bit RGBA representation */
        };

        static const Color Black; /*!< Opaque black (0, 0, 0, 255) */
        static const Color White; /*!< Opaque white (255, 255, 255, 255) */
        static const Color Red; /*!< Opaque red (255, 0, 0, 255) */
        static const Color Green; /*!< Opaque green (0, 255, 0, 255) */
        static const Color Blue; /*!< Opaque blue (0, 0, 255, 255) */
        static const Color Transparent; /*!< Fully transparent (0, 0, 0, 0) */

        /**
         * \brief Default constructor
         *
         * Initializes the color to fully transparent black.
         */
        constexpr Color() :
            Bits(0)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a color from a packed 32-bit value
         *
         * \param bits Packed RGBA value
         */
        explicit Color(Uint32 bits) :
            Bits(bits)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a color from individual channel values
         *
         * \param r Value of the red channel
         * \param g Value of the green channel
         * \param b Value of the blue channel
         * \param a Value of the alpha channel (defaults to 255)
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
         * \brief Returns a copy of the color with a modified alpha channel
         *
         * \param alpha New alpha value
         *
         * \return Color with updated alpha
         */
        Color WithAlpha(Uint8 alpha) const;

        /**
         * \brief Indicates whether the color is fully opaque
         *
         * \return \c true if alpha equals 255, \c false otherwise
         */
        bool IsOpaque() const;

        /**
         * \brief Indicates whether the color has any transparency
         *
         * \return \c true if alpha is less than 255, \c false otherwise
         */
        bool IsTransparent() const;

        /**
         * \brief Indicates whether the color is fully transparent
         *
         * \return \c true if alpha equals 0, \c false otherwise
         */
        bool IsFullyTransparent() const;

        /**
         * \brief Converts the color to linear color space
         *
         * Interprets the color as sRGB and converts it to a \ref LinearColor
         * suitable for lighting and color computations.
         *
         * \return Linear color representation
         */
        LinearColor ToLinear() const;

        /**
         * \brief Returns the color packed in RGBA byte order
         *
         * \return Packed RGBA 32-bit value
         */
        Uint32 ToPackedRGBA() const;

        /**
         * \brief Returns the color packed in BGRA byte order
         *
         * \return Packed BGRA 32-bit value
         */
        Uint32 ToPackedBGRA() const;

        /**
         * \brief Returns the color packed in ARGB byte order
         *
         * \return Packed ARGB 32-bit value
         */
        Uint32 ToPackedARGB() const;

        /**
         * \brief Returns the color packed in ABGR byte order
         *
         * \return Packed ABGR 32-bit value
         */
        Uint32 ToPackedABGR() const;

        /**
         * \brief Equality comparison operator
         *
         * \param rhs Color to compare with
         *
         * \return \c true if both colors are identical
         */
        bool operator==(const Color& rhs) const;

        /**
         * \brief Inequality comparison operator
         *
         * \param rhs Color to compare with
         *
         * \return \c true if the colors differ
         */
        bool operator!=(const Color& rhs) const;
    };

    /**
     * \brief Represents a color in linear floating-point space
     *
     * LinearColor stores color components as floating-point values in linear
     * color space. It is primarily used for lighting, shading and color
     * computations where linear behavior is required.
     */
    class COCKTAIL_CORE_API LinearColor
    {
    public:

        union
        {
            struct
            {
                float R, G, B, A; /*!< Red, Green, Blue and Alpha components */
            };
            float RGBA[4]; /*!< Array access to color components */
        };

        static const LinearColor Black; /*!< Opaque black (0.f, 0.f, 0.f, 1.f) */
        static const LinearColor White; /*!< Opaque white (1.f, 1.f, 1.f, 1.f) */
        static const LinearColor Red; /*!< Opaque red (1.f, 0.f, 0.f, 1.f) */
        static const LinearColor Green; /*!< Opaque green (0.f, 1.f, 0.f, 1.f) */
        static const LinearColor Blue; /*!< Opaque blue (0.f, 0.f, 1.f, 1.f) */
        static const LinearColor Transparent; /*!< Fully transparent (0.f, 0.f, 0.f, 0.f) */

        /**
         * \brief Default constructor
         *
         * Initializes the color to transparent black.
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
         * \brief Construct a linear color by converting a sRGB color
         *
         * \param color The color to convert
         */
        explicit LinearColor(const Color& color);

        /**
         * \brief Constructs a linear color from components
         *
         * \param r Red component
         * \param g Green component
         * \param b Blue component
         * \param a Alpha component (defaults to 1.0)
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
         * \brief Computes the luminance of the color
         *
         * Uses standard luminance coefficients to compute perceived brightness.
         *
         * \return Luminance value
         */
        float GetLuminance() const;

        /**
         * \brief Converts the linear color to an 8-bit color
         *
         * Converts the color to \ref Color, optionally applying sRGB gamma
         * correction.
         *
         * \param sRgb Whether to apply sRGB gamma correction
         *
         * \return Converted 8-bit color
         */
        Color ToColor(bool sRgb) const;

        /**
         * \brief Equality comparison operator
         *
         * \param rhs Color to compare with
         *
         * \return \c true if both colors are identical
         */
        bool operator==(const LinearColor& rhs) const;

        /**
         * \brief Inequality comparison operator
         *
         * \param rhs Color to compare with
         *
         * \return \c true if the colors differ
         */
        bool operator!=(const LinearColor& rhs) const;
    };
}

#endif // COCKTAIL_CORE_COLOR_HPP
