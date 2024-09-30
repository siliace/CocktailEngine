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
		e0, /*!< Specifies no color channel */
		e8, /*!< Specifies a color depth of 8 bits */
		e16, /*!< Specifies a color depth of 16 bits */
		e24, /*!< Specifies a color depth of 24 bits */
		e32, /*!< Specifies a color depth of 32 bits */
		e48, /*!< Specifies a color depth of 48 bits */
		e64, /*!< Specifies a color depth of 64 bits */
		e96, /*!< Specifies a color depth of 96 bits */
		e128, /*!< Specifies a color depth of 128 bits */
		e192, /*!< Specifies a color depth of 192 bits */
		e256, /*!< Specifies a color depth of 256 bits */
	};

	/**
	 * \brief Enumeration of possible number of bits in the alpha channel
	 */
	enum class DisplayAlphaDepth
	{
		e0, /*!< Specifies no alpha channel */
		e2, /*!< Specifies an alpha channel of 2 bits */
		e8, /*!< Specifies an alpha channel of 8 bits */
		e16, /*!< Specifies an alpha channel of 16 bits */
        e32, /*!< Specifies an alpha channel of 32 bits */
        e64, /*!< Specifies an alpha channel of 64 bits */
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
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP
