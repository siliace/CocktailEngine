#ifndef COCKTAILENGINE_RENDERER_FORMAT_HPP
#define COCKTAILENGINE_RENDERER_FORMAT_HPP

#include <CocktailEngine/Renderer/PixelFormat.hpp>
#include <CocktailEngine/Core/System/Monitor/VideoMode.hpp>

#include <CocktailEngine/Renderer/ColorSpace.hpp>
#include <CocktailEngine/Renderer/Export.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 * \param pixelFormat 
	 * \return 
	 */
	COCKTAILENGINE_RENDERER_API DisplayColorDepth FormatToColorDepth(const PixelFormat& pixelFormat);

	/**
	 * \brief 
	 * \param pixelFormat 
	 * \return 
	 */
	COCKTAILENGINE_RENDERER_API DisplayAlphaDepth FormatToAlphaDepth(const PixelFormat& pixelFormat);

	/**
	 * \brief 
	 * \param pixelFormat 
	 * \return 
	 */
	COCKTAILENGINE_RENDERER_API ColorSpace FormatToColorSpace(const PixelFormat& pixelFormat);
}

#endif // COCKTAILENGINE_RENDERER_FORMAT_HPP
