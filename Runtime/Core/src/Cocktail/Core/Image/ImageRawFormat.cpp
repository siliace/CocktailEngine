#include <Cocktail/Core/Image/ImageRawFormat.hpp>

namespace Ck
{
    namespace
    {
        template <typename T>
        LinearColor GreyPixelMemoryToColor(const T* pixel, GammaSpace gammaSpace, bool hasAlpha)
        {
            static constexpr auto Max = std::numeric_limits<T>::max();

            LinearColor color;
            const float normalized = static_cast<float>(pixel[0]) / Max;
            const float grey = gammaSpace == GammaSpace::sRGB ? ColorConversion::SrgbColorToLinear(normalized) : normalized;

            color.R = grey;
            color.B = grey;
            color.G = grey;
            color.A = hasAlpha ? static_cast<float>(pixel[1]) / Max : 1.f;

            return color;
        }

        template <typename T>
        LinearColor RgbPixelMemoryToColor(const T* pixel, GammaSpace gammaSpace, bool hasAlpha)
        {
            static constexpr auto Max = std::numeric_limits<T>::max();

            LinearColor color;
            color.R = static_cast<float>(pixel[0]) / Max;
            color.G = static_cast<float>(pixel[1]) / Max;
            color.B = static_cast<float>(pixel[2]) / Max;
            color.A = hasAlpha ? static_cast<float>(pixel[3]) / Max : 1.f;

            if (gammaSpace == GammaSpace::sRGB)
            {
                color.R = ColorConversion::SrgbColorToLinear(color.R);
                color.G = ColorConversion::SrgbColorToLinear(color.G);
                color.B = ColorConversion::SrgbColorToLinear(color.B);
            }

            return color;
        }
    }

    ImageRawFormat::Type ImageRawFormat::Of(unsigned int channelCount, DataType type)
    {
        if (type == DataType::UnsignedInt8)
        {
            switch (channelCount)
            {
                case 1: return Type::Grey8;
                case 2: return Type::GreyAlpha8;
                case 3: return Type::RedGreenBlue8;
                case 4: return Type::RedGreenBlueAlpha8;
            }
        }
        else if (type == DataType::UnsignedInt16)
        {
            switch (channelCount)
            {
                case 1: return Type::Grey16;
                case 2: return Type::GreyAlpha16;
                case 3: return Type::RedGreenBlue16;
                case 4: return Type::RedGreenBlueAlpha16;
            }
        }
        else if (type == DataType::Float32)
        {
            switch (channelCount)
            {
                case 1: return Type::Grey32F;
                case 2: return Type::GreyAlpha32F;
                case 3: return Type::RedGreenBlue32F;
                case 4: return Type::RedGreenBlueAlpha32F;
            }
        }

        COCKTAIL_UNREACHABLE();
    }

    LinearColor ImageRawFormat::GetPixelColor(const Byte* pixel, Type format, GammaSpace gammaSpace)
    {
        assert(pixel);

        switch (format)
        {
            case Type::Grey8: return GreyPixelMemoryToColor(pixel, gammaSpace, false);
            case Type::Grey16: return GreyPixelMemoryToColor(reinterpret_cast<const Uint16*>(pixel), gammaSpace, false);
            case Type::Grey32F: return GreyPixelMemoryToColor(reinterpret_cast<const float*>(pixel), gammaSpace, false);

            case Type::GreyAlpha8: return GreyPixelMemoryToColor(pixel, gammaSpace, true);
            case Type::GreyAlpha16: return GreyPixelMemoryToColor(reinterpret_cast<const Uint16*>(pixel), gammaSpace, true);
            case Type::GreyAlpha32F: return GreyPixelMemoryToColor(reinterpret_cast<const float*>(pixel), gammaSpace, true);

            case Type::RedGreenBlue8: return RgbPixelMemoryToColor(pixel, gammaSpace, false);
            case Type::RedGreenBlue16: return RgbPixelMemoryToColor(reinterpret_cast<const Uint16*>(pixel), gammaSpace, false);
            case Type::RedGreenBlue32F: return RgbPixelMemoryToColor(reinterpret_cast<const float*>(pixel), gammaSpace, false);

            case Type::RedGreenBlueAlpha8: return RgbPixelMemoryToColor(pixel, gammaSpace, true);
            case Type::RedGreenBlueAlpha16: return RgbPixelMemoryToColor(reinterpret_cast<const Uint16*>(pixel), gammaSpace, true);
            case Type::RedGreenBlueAlpha32F: return RgbPixelMemoryToColor(reinterpret_cast<const float*>(pixel), gammaSpace, true);
        }

        COCKTAIL_UNREACHABLE();
    }

    unsigned int ImageRawFormat::GetChannelCount(Type format)
    {
        switch (format)
        {
            case Type::Grey8:
            case Type::Grey16:
            case Type::Grey32F: return 1;

            case Type::GreyAlpha8:
            case Type::GreyAlpha16:
            case Type::GreyAlpha32F: return 1;

            case Type::RedGreenBlue8:
            case Type::RedGreenBlue16:
            case Type::RedGreenBlue32F: return 3;

            case Type::RedGreenBlueAlpha8:
            case Type::RedGreenBlueAlpha16:
            case Type::RedGreenBlueAlpha32F: return 4;
        }

        COCKTAIL_UNREACHABLE();
    }

    unsigned int ImageRawFormat::GetChannelStride(Type format)
    {
        switch (format)
        {
            case Type::Grey8:
            case Type::GreyAlpha8:
            case Type::RedGreenBlue8:
            case Type::RedGreenBlueAlpha8: return 1;

            case Type::Grey16:
            case Type::GreyAlpha16:
            case Type::RedGreenBlue16:
            case Type::RedGreenBlueAlpha16: return 2;

            case Type::Grey32F:
            case Type::GreyAlpha32F:
            case Type::RedGreenBlue32F:
            case Type::RedGreenBlueAlpha32F: return 4;
        }

        COCKTAIL_UNREACHABLE();
    }

    Uint64 ImageRawFormat::GetPixelStride(Type format)
    {
        return GetChannelCount(format) * GetChannelStride(format);
    }

    bool ImageRawFormat::IsGrey(Type format)
    {
        switch (format)
        {
            case Type::Grey8:
            case Type::Grey16:
            case Type::Grey32F:
            case Type::GreyAlpha8:
            case Type::GreyAlpha16:
            case Type::GreyAlpha32F: return true;
        }

        return false;
    }

    bool ImageRawFormat::IsColor(Type format)
    {
        switch (format)
        {
            case Type::RedGreenBlue8:
            case Type::RedGreenBlue16:
            case Type::RedGreenBlue32F:
            case Type::RedGreenBlueAlpha8:
            case Type::RedGreenBlueAlpha16:
            case Type::RedGreenBlueAlpha32F: return true;
        }

        return false;
    }

    bool ImageRawFormat::IsHdr(Type format)
    {
        switch (format)
        {
            case Type::Grey32F:
            case Type::GreyAlpha32F:
            case Type::RedGreenBlue32F:
            case Type::RedGreenBlueAlpha32F: return true;
        }

        return false;
    }

    bool ImageRawFormat::HasAlphaChannel(Type format)
    {
        switch (format)
        {
            case Type::GreyAlpha8:
            case Type::GreyAlpha16:
            case Type::GreyAlpha32F:
            case Type::RedGreenBlueAlpha8:
            case Type::RedGreenBlueAlpha16:
            case Type::RedGreenBlueAlpha32F: return true;
        }

        return false;
    }
}
