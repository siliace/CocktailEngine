#ifndef COCKTAIL_RENDERER_FORMAT_HPP
#define COCKTAIL_RENDERER_FORMAT_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/System/Monitor/VideoMode.hpp>

#include <Cocktail/Renderer/ColorSpace.hpp>
#include <Cocktail/Renderer/Export.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 * \param pixelFormat 
	 * \return 
	 */
	COCKTAIL_RENDERER_API DisplayColorDepth FormatToColorDepth(const PixelFormat& pixelFormat);

	/**
	 * \brief 
	 * \param pixelFormat 
	 * \return 
	 */
	COCKTAIL_RENDERER_API DisplayAlphaDepth FormatToAlphaDepth(const PixelFormat& pixelFormat);

	/**
	 * \brief 
	 * \param pixelFormat 
	 * \return 
	 */
	COCKTAIL_RENDERER_API ColorSpace FormatToColorSpace(const PixelFormat& pixelFormat);
}

#endif // COCKTAIL_RENDERER_FORMAT_HPP
