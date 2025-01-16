#include <cassert>
#include <tuple>

#include <Cocktail/Core/Image/PixelFormat.hpp>

namespace Ck
{
	PixelFormat PixelFormat::Undefined()
	{
		return {};
	}

	PixelFormat PixelFormat::DepthStencil(unsigned int depthBits, unsigned int stencilBits)
	{
		if (depthBits)
		{
			if (stencilBits)
			{
				Channel channels[2] = {
					{ depthBits, false, true },
					{ stencilBits, true, false }
				};

				return PixelFormat(Layout::DepthStencil, channels, Encoding::Raw);
			}

			Channel channels[1] = {
				{ depthBits, false, true },
			};

			return PixelFormat(Layout::Depth, channels, Encoding::Raw);
		}

		Channel channels[1] = {
			{ stencilBits, true, false },
		};

		return PixelFormat(Layout::Stencil, channels, Encoding::Raw);
	}

	PixelFormat PixelFormat::Compressed(CompressionScheme compressionScheme, bool srgb)
	{
		return PixelFormat(compressionScheme, srgb);
	}

	PixelFormat PixelFormat::Color(Layout layout, const DataType& dataType, Encoding encoding)
	{
		Channel channel;
		channel.Length = static_cast<unsigned>(dataType.GetSize()) * 8llu;
		channel.IsUnsigned = dataType.IsUnsigned();
		channel.IsFloatingPoint = dataType.IsFloatingPoint();

		Channel channels[MaxChannelCount];
		for (unsigned int i = 0; i < MaxChannelCount; i++)
			channels[i] = channel;

		return Color(layout, channels, encoding);
	}

	PixelFormat PixelFormat::Color(Layout layout, const Channel* channels, Encoding encoding)
	{
		return PixelFormat(layout, channels, encoding);
	}

	PixelFormat::PixelFormat() :
		mLayout(Layout::None),
		mEncoding(Encoding::None),
		mCompressionScheme(CompressionScheme::None)
	{
		/// Nothing
	}

	std::size_t PixelFormat::ComputeAllocationSize(const Extent2D<unsigned int>& size) const
	{
		std::size_t allocationSize;
		if (IsCompressed())
		{
			allocationSize = ((size.Width + 3) / 4) * ((size.Height + 3) / 4);
		}
		else
		{
			allocationSize = size.Width * size.Height;
		}
		allocationSize *= GetBlockSize();

		assert(allocationSize != 0);
		return allocationSize;
	}

	std::size_t PixelFormat::ComputeAllocationSize(const Extent3D<unsigned>& size) const
	{
		std::size_t allocationSize;
		if (IsCompressed())
		{
			allocationSize = (size.Width + 3) / 4 * ((size.Height + 3) / 4) * ((size.Depth + 3) / 4);
		}
		else
		{
			allocationSize = size.Width * size.Height * size.Depth;
		}
		allocationSize *= GetBlockSize();

		assert(allocationSize != 0);
		return allocationSize;
	}

	PixelFormat::Layout PixelFormat::GetLayout() const
	{
		return mLayout;
	}

	unsigned PixelFormat::GetChannelCount() const
	{
		switch (mLayout)
		{
		case Layout::None:
			return 0;

		case Layout::R:
		case Layout::Depth:
		case Layout::Stencil:
			return 1;

		case Layout::RG:
		case Layout::DepthStencil:
			return 2;

		case Layout::RGB:
		case Layout::BGR:
			return 3;

		case Layout::RGBA:
		case Layout::BGRA:
		case Layout::ARGB:
		case Layout::ABGR:
			return 4;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	const PixelFormat::Channel* PixelFormat::GetChannel(unsigned int index) const
	{
		if (index >= GetChannelCount())
			return nullptr;

		return &mChannels[index];
	}

	Extent2D<unsigned> PixelFormat::GetBlockExtent() const
	{
		const unsigned int size = IsCompressed() ? 4 : 1;
		return MakeExtent(size, size);
	}

	std::size_t PixelFormat::GetBlockSize() const
	{
		if (IsCompressed())
		{
			bool isDxt1 = mCompressionScheme == CompressionScheme::Dxt1;
			isDxt1 |= mCompressionScheme == CompressionScheme::AlphaDxt1;
			return isDxt1 ? 8 : 16;
		}

		std::size_t blockSize = 0;
		for (unsigned int i = 0; i < GetChannelCount(); i++)
			blockSize += mChannels[i].Length;

		return blockSize / 8;
	}

	bool PixelFormat::IsCompressed() const
	{
		return mCompressionScheme != CompressionScheme::None;
	}

	bool PixelFormat::IsColor() const
	{
		return mLayout != Layout::Depth && 
			mLayout != Layout::Stencil &&
			mLayout != Layout::DepthStencil;
	}

	bool PixelFormat::IsDepth() const
	{
		return mLayout == Layout::Depth;
	}

	bool PixelFormat::IsStencil() const
	{
		return mLayout == Layout::Stencil;
	}

	bool PixelFormat::IsDepthStencil() const
	{
		return mLayout == Layout::DepthStencil;
	}

	PixelFormat::Encoding PixelFormat::GetEncoding() const
	{
		return mEncoding;
	}

	PixelFormat::CompressionScheme PixelFormat::GetCompressionScheme() const
	{
		return mCompressionScheme;
	}

	bool PixelFormat::operator==(const PixelFormat& rhs) const
	{
		if (mLayout != rhs.GetLayout())
			return false;

		for (unsigned int i = 0; i < GetChannelCount(); i++)
		{
			const Channel& lhsChannel = mChannels[i];
			const Channel& rhsChannel = rhs.mChannels[i];

			if (std::tie(lhsChannel.Length, lhsChannel.IsUnsigned, lhsChannel.IsFloatingPoint) !=
				std::tie(rhsChannel.Length, rhsChannel.IsUnsigned, rhsChannel.IsFloatingPoint))
				return false;
		}

		if (mEncoding != rhs.GetEncoding())
			return false;

		if (mCompressionScheme != rhs.GetCompressionScheme())
			return false;

		return true;
	}

	bool PixelFormat::operator!=(const PixelFormat& rhs) const
	{
		return !(*this == rhs);
	}

	PixelFormat::PixelFormat(Layout layout, const Channel* channels, Encoding encoding):
		mLayout(layout),
		mEncoding(encoding),
		mCompressionScheme(CompressionScheme::None)
	{
		assert(mLayout != Layout::None);
		assert(mEncoding != Encoding::None);
		for (unsigned int i = 0; i < GetChannelCount(); i++)
			mChannels[i] = channels[i];
	}

	PixelFormat::PixelFormat(CompressionScheme compressionScheme, bool srgb):
		mCompressionScheme(compressionScheme)
	{
		assert(mCompressionScheme != CompressionScheme::None);
		mLayout = mCompressionScheme == CompressionScheme::Dxt1 ? Layout::RGB : Layout::RGBA;
		mEncoding = srgb ? Encoding::Srgb : Encoding::Raw;
	}
}
