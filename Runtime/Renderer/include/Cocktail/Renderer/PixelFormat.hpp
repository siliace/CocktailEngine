#ifndef COCKTAIL_RENDERER_PIXELFORMAT_HPP
#define COCKTAIL_RENDERER_PIXELFORMAT_HPP

#include <array>

#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Extent3D.hpp>
#include <Cocktail/Core/Image/ImageRawFormat.hpp>

#include <Cocktail/Renderer/Export.hpp>

namespace Ck
{
    /**
     * \brief Class describing the memory organization of a texel
     *
     * PixelFormat describes how a single texel is stored in memory. It defines
     * the channel layout (order and presence of channels), the data type and
     * encoding of each channel, and optional compression schemes.
     *
     * This class is format-agnostic and is primarily used by the rendering
     * backend to describe GPU-compatible image formats.
     */
    class COCKTAIL_RENDERER_API PixelFormat
    {
    public:

        /**
         * \brief Maximum number of channels supported per texel
         */
        static constexpr unsigned int MaxChannelCount = 4;

        /**
         * \brief Enumeration of supported channel layouts
         *
         * The layout describes which channels are present in a texel and
         * their order in memory.
         */
        enum class Layout
        {
            None, /*!< Undefined or invalid layout */
            R, /*!< Single red channel */
            RG, /*!< Red-green layout */
            RGB, /*!< Red-green-blue layout */
            RGBA, /*!< Red-green-blue-alpha layout */
            ARGB, /*!< Alpha-red-green-blue layout */
            BGR, /*!< Blue-green-red layout */
            BGRA, /*!< Blue-green-red-alpha layout */
            ABGR, /*!< Alpha-blue-green-red layout */
            Depth, /*!< Depth-only format */
            Stencil, /*!< Stencil-only format */
            DepthStencil /*!< Combined depth-stencil format */
        };

        /**
         * \brief Enumeration of supported channel encodings
         *
         * The encoding specifies how raw channel values should be interpreted.
         */
        enum class Encoding
        {
            None, /*!< No encoding defined */
            Normalized, /*!< Normalized integer values */
            Raw, /*!< Raw integer or floating-point values */
            Scaled, /*!< Scaled integer values */
            Srgb, /*!< sRGB gamma-corrected values */
        };

        /**
         * \brief Enumeration of supported compression schemes
         */
        enum class CompressionScheme
        {
            None, /*!< Uncompressed format */
            Dxt1, /*!< DXT1 compression */
            AlphaDxt1, /*!< DXT1 compression with alpha */
            Dxt3, /*!< DXT3 compression */
            Dxt5 /*!< DXT5 compression */
        };

        /**
         * \brief Description of a single channel
         */
        struct Channel
        {
            unsigned int Length = 0; /*!< Bit size of the channel */
            bool IsUnsigned = false; /*!< Whether the channel is unsigned */
            bool IsFloatingPoint = false; /*!< Whether the channel is floating-point */
        };

        /**
         * \brief Create an undefined pixel format
         *
         * \return An undefined pixel format instance
         */
        static PixelFormat Undefined();

        /**
         * \brief Create a depth-stencil pixel format
         *
         * \param depthBits Number of bits used for the depth channel
         * \param stencilBits Number of bits used for the stencil channel
         *
         * \return A depth-stencil pixel format
         */
        static PixelFormat DepthStencil(unsigned int depthBits, unsigned int stencilBits);

        /**
         * \brief Create a compressed pixel format
         *
         * \param compressionScheme Compression scheme to use
         * \param srgb Whether the format uses sRGB encoding
         *
         * \return A compressed pixel format
         */
        static PixelFormat Compressed(CompressionScheme compressionScheme, bool srgb);

        /**
         * \brief Create a color pixel format with homogeneous channel types
         *
         * Creates a color pixel format where all channels share the same data
         * type and encoding.
         *
         * \param layout Channel layout of the pixel format
         * \param dataType Data type of each channel
         * \param encoding Encoding applied to channel values
         *
         * \return The created pixel format
         */
        static PixelFormat Color(Layout layout, const DataType& dataType, Encoding encoding = Encoding::Normalized);

        /**
         * \brief Create a color pixel format with explicit channel descriptions
         *
         * Allows full control over each channel's bit size and numeric representation.
         *
         * \param layout Channel layout of the pixel format
         * \param channels Array describing each channel
         * \param encoding Encoding applied to channel values
         *
         * \return The created pixel format
         */
        static PixelFormat Color(Layout layout, const Channel* channels, Encoding encoding = Encoding::Normalized);

        /**
         * \brief Create a pixel format from an image raw format
         *
         * Converts an \ref ImageRawFormat into a corresponding render-compatible
         * pixel format, taking gamma space into account.
         *
         * \param imageRawFormat Source image raw format
         * \param gammaSpace Gamma space of the image
         *
         * \return The corresponding pixel format
         */
        static PixelFormat FromImageRawFormat(ImageRawFormat::Type imageRawFormat, GammaSpace gammaSpace);

        /**
         * \brief Default constructor
         *
         * Creates an undefined pixel format.
         *
         * \see PixelFormat::Undefined
         */
        PixelFormat();

        /**
         * \brief Computes the memory size required for a 2D image
         *
         * \param size Image dimensions in pixels
         *
         * \return Required allocation size in bytes
         */
        std::size_t ComputeAllocationSize(const Extent2D<unsigned int>& size) const;

        /**
         * \brief Computes the memory size required for a 3D image
         *
         * \param size Image dimensions in pixels
         *
         * \return Required allocation size in bytes
         */
        std::size_t ComputeAllocationSize(const Extent3D<unsigned int>& size) const;

        /**
         * \brief Returns the channel layout of the pixel format
         */
        Layout GetLayout() const;

        /**
         * \brief Returns the number of channels in the format
         */
        unsigned int GetChannelCount() const;

        /**
         * \brief Returns the description of a specific channel
         *
         * \param index Channel index
         *
         * \return Pointer to the channel description, or nullptr if invalid
         */
        const Channel* GetChannel(unsigned int index) const;

        /**
         * \brief Returns the extent of a single texel block
         *
         * For uncompressed formats, the block extent is typically (1, 1).
         * For compressed formats, this corresponds to the compression block size.
         *
         * \return Block extent in texels
         */
        Extent2D<unsigned int> GetBlockExtent() const;

        /**
         * \brief Returns the size in bytes of a single texel block
         *
         * For uncompressed formats, this corresponds to the size of a single pixel.
         * For compressed formats, this is the size of one compression block.
         *
         * \return Size in bytes of one block
         */
        std::size_t GetBlockSize() const;

        /**
         * \brief Indicates whether the pixel format uses compression
         *
         * \return \c true if the format is compressed, \c false otherwise
         */
        bool IsCompressed() const;

        /**
         * \brief Indicates whether the pixel format represents color data
         *
         * \return \c true if the format is a color format, \c false otherwise
         */
        bool IsColor() const;

        /**
         * \brief Indicates whether the pixel format contains a depth component
         *
         * \return \c true if the format contains depth data, \c false otherwise
         */
        bool IsDepth() const;

        /**
         * \brief Indicates whether the pixel format contains a stencil component
         *
         * \return \c true if the format contains stencil data, \c false otherwise
         */
        bool IsStencil() const;

        /**
         * \brief Indicates whether the pixel format is a combined depth-stencil format
         *
         * \return \c true if the format contains both depth and stencil components,
         *         \c false otherwise
         */
        bool IsDepthStencil() const;

        /**
         * \brief Returns the encoding used by the pixel format
         *
         * \return Encoding applied to channel values
         */
        Encoding GetEncoding() const;

        /**
         * \brief Returns the compression scheme used by the pixel format
         *
         * \return Compression scheme of the format, or CompressionScheme::None
         */
        CompressionScheme GetCompressionScheme() const;

        /**
         * \brief Compares this pixel format with another for equality
         *
         * Two pixel formats are considered equal if they share the same layout,
         * channel descriptions, encoding and compression scheme.
         *
         * \param rhs Pixel format to compare against
         *
         * \return \c true if both formats are identical, \c false otherwise
         */
        bool operator==(const PixelFormat& rhs) const;

        /**
         * \brief Compares this pixel format with another for inequality
         *
         * \param rhs Pixel format to compare against
         *
         * \return \c true if the formats differ, \c false otherwise
         */
        bool operator!=(const PixelFormat& rhs) const;

    private:

        /**
         * \brief Constructs a color pixel format
         *
         * Creates a pixel format with the given channel layout, channel descriptions
         * and encoding. This constructor is used internally by the static factory
         * functions.
         *
         * \param layout Channel layout of the pixel format
         * \param channels Array describing each channel of the format
         * \param encoding Encoding applied to channel values
         */
        PixelFormat(Layout layout, const Channel* channels, Encoding encoding);

        /**
         * \brief Constructs a compressed pixel format
         *
         * Creates a pixel format using the specified compression scheme. The layout
         * and channel information are derived from the compression format.
         *
         * \param compressionScheme Compression scheme used by the format
         * \param srgb Indicates whether the compressed data is stored in sRGB space
         */
        PixelFormat(CompressionScheme compressionScheme, bool srgb);

        Layout mLayout;
        std::array<Channel, MaxChannelCount> mChannels;
        Encoding mEncoding;
        CompressionScheme mCompressionScheme;
    };
}

#endif // COCKTAIL_RENDERER_PIXELFORMAT_HPP
