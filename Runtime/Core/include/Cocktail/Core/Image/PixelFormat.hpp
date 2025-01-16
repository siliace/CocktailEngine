#ifndef COCKTAIL_CORE_IMAGE_PIXELFORMAT_HPP
#define COCKTAIL_CORE_IMAGE_PIXELFORMAT_HPP

#include <array>

#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Extent3D.hpp>

namespace Ck
{
	/**
	 * \brief Class describing memory organization of a texel
	 */
	class COCKTAIL_CORE_API PixelFormat
	{
	public:

		/**
		 * \brief The number of maximal channel per texel supported
		 */
		static constexpr unsigned int MaxChannelCount = 4;

		/**
		 * \brief Enumeration of supported classes of formats
		 * These class describe which channels are presents into a texel
		 */
		enum class Layout
		{
			None,
			R,
			RG,
			RGB,
			RGBA,
			ARGB,
			BGR,
			BGRA,
			ABGR,
			Depth,
			Stencil,
			DepthStencil
		};

		/**
		 * \brief Enumeration of supported
		 */
		enum class Encoding
		{
			None,
			Normalized,
			Raw,
			Scaled,
			Srgb,
		};


		/**
		 * \brief Enumeration of supported
		 */
		enum class CompressionScheme
		{
			None,
			Dxt1,
			AlphaDxt1,
			Dxt3,
			Dxt5
		};

		struct Channel
		{
			unsigned int Length = 0;
			bool IsUnsigned = false;
			bool IsFloatingPoint = false;
		};

		/**
		 * \brief 
		 * \return 
		 */
		static PixelFormat Undefined();

		/**
		 * \brief 
		 * \param depthBits 
		 * \param stencilBits 
		 * \return 
		 */
		static PixelFormat DepthStencil(unsigned int depthBits, unsigned int stencilBits);

		/**
		 * \brief
		 * \param compressionScheme
		 * \param srgb
		 */
		static PixelFormat Compressed(CompressionScheme compressionScheme, bool srgb);

		/**
		 * \brief Create a color pixel format with a given layout with all channels sharing the same type
		 * \param layout The layout of the pixel format to create
		 * \param dataType The data type of every channel supported by the layout
		 * \param encoding The encoding method of texel images using this format will receive
		 * \return The create pixel format
		 */
		static PixelFormat Color(Layout layout, const DataType& dataType, Encoding encoding = Encoding::Normalized);

		/**
		 * \brief 
		 * \param layout 
		 * \param channels 
		 * \param encoding 
		 * \return 
		 */
		static PixelFormat Color(Layout layout, const Channel* channels, Encoding encoding = Encoding::Normalized);

		/**
		 * \brief Default constructor
		 * Used to create undefined pixel format
		 * \see PixelFormat::Undefined
		 */
		PixelFormat();
		
		/**
		 * \brief Compute the size (in bytes) needed to store pixels of a given side
		 * \param size 
		 * \return 
		 */
		std::size_t ComputeAllocationSize(const Extent2D<unsigned int>& size) const;

		/**
		 * \brief Compute the size (in bytes) needed to store pixels of a given side
		 * \param size
		 * \return
		 */
		std::size_t ComputeAllocationSize(const Extent3D<unsigned int>& size) const;

		/**
		 * \brief 
		 * \return 
		 */
		Layout GetLayout() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetChannelCount() const;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const Channel* GetChannel(unsigned int index) const;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned int> GetBlockExtent() const;

		/**
		 * \brief Get the size, in bytes of a single block of texel using this format
		 * \return The size in byte
		 */
		std::size_t GetBlockSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsCompressed() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsColor() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsDepth() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsStencil() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsDepthStencil() const;

		/**
		 * \brief 
		 * \return 
		 */
		Encoding GetEncoding() const;

		/**
		 * \brief 
		 * \return 
		 */
		CompressionScheme GetCompressionScheme() const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator==(const PixelFormat& rhs) const;

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		bool operator!=(const PixelFormat& rhs) const;

	private:

		/**
		 * \brief Constructor
		 * Create a new pixel format
		 * \param layout 
		 * \param channels 
		 * \param encoding 
		 */
		PixelFormat(Layout layout, const Channel* channels, Encoding encoding);

		/**
		 * \brief Constructor
		 * Create a new compressed pixel format
		 * \param compressionScheme 
		 * \param srgb 
		 */
		PixelFormat(CompressionScheme compressionScheme, bool srgb);

		Layout mLayout;
		std::array<Channel, MaxChannelCount> mChannels;
		Encoding mEncoding;
		CompressionScheme mCompressionScheme;
	};
}

#endif // COCKTAIL_CORE_IMAGE_PIXELFORMAT_HPP
