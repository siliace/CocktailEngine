#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP

#include <Cocktail/Core/Extent2D.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	enum class DisplayColorDepth
	{
		e24, /*!< Specifies a color depth of 24 bits */
		e32, /*!< Specifies a color depth of 32 bits */
	};

	/**
	 * \brief Enumeration of possible number of bits in the alpha channel
	 */
	enum class DisplayAlphaDepth
	{
		e0, /*!< Specifies no alpha channel */
		e8, /*!< Specifies an alpha channel of 8 bits */
	};

	/**
	 * \brief 
	 */
	struct VideoMode
	{
		/**
		 * \brief 
		 */
		Extent2D<unsigned int> Size;

		/**
		 * \brief 
		 */
		unsigned int Frequency = 0;

		/**
		 * \brief 
		 */
		DisplayColorDepth ColorDepth = DisplayColorDepth::e32;

		/**
		 * \brief
		 */
		DisplayAlphaDepth AlphaDepth = DisplayAlphaDepth::e8;

		bool operator==(const VideoMode& rhs) const
		{
			return std::tie(Size, Frequency, ColorDepth, AlphaDepth) == 
				std::tie(rhs.Size, rhs.Frequency, rhs.ColorDepth, rhs.AlphaDepth);
		}

		bool operator!=(const VideoMode& rhs) const
		{
			return !(*this == rhs);
		}
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP
