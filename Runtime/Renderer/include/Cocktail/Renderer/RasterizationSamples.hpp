#ifndef COCKTAIL_RENDERER_RASTERIZATIONSAMPLES_HPP
#define COCKTAIL_RENDERER_RASTERIZATIONSAMPLES_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration of possible multisampling values
	 */
	enum class RasterizationSamples
	{
		/**
		 * \brief Specifies a multisampling with 1 sample per pixel
		 */
		e1 = Bit(0),

		/**
		 * \brief Specifies a multisampling with 2 samples per pixel
		 */
		e2 = Bit(1),

		/**
		 * \brief Specifies a multisampling with 4 samples per pixel
		 */
		e4 = Bit(2),

		/**
		 * \brief Specifies a multisampling with 8 samples per pixel
		 */
		e8 = Bit(3),

		/**
		 * \brief Specifies a multisampling with 16 samples per pixel
		 */
		e16 = Bit(4),

		/**
		 * \brief Specifies a multisampling with 32 samples per pixel
		 */
		e32 = Bit(5),

		/**
		 * \brief Specifies a multisampling with 64 samples per pixel
		 */
		e64 = Bit(6),
	};
}

#endif // COCKTAIL_RENDERER_RASTERIZATIONSAMPLES_HPP
