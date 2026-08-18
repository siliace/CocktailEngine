#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP

#include <CocktailEngine/Core/System/Window/ImageCursor.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursor.hpp>
#include <CocktailEngine/Core/System/Window/SystemCursorType.hpp>
#include <CocktailEngine/Core/System/Window/Window.hpp>
#include <CocktailEngine/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck
{
	/**
	 * \brief Base interface exposing Cursor, Window and WindowEventQueue creation
	 */
	class WindowFactory
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~WindowFactory() = default;

		/**
		 * \brief Create a new Cursor
		 *
		 * \param image
		 * \param hotspot
		 *
		 * \return The created Cursor
		 */
		virtual UniquePtr<ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) = 0;

		/**
		 * \brief Create a new Window
		 *
		 * \param createInfo The structure describing the Window to create
		 *
		 * \return The created Window
		 */
		virtual UniquePtr<Window> CreateWindow(const WindowCreateInfo& createInfo) = 0;

		/**
		 * \brief Load a system's Cursor
		 *
		 * \param type The type of Cursor to load
		 *
		 * \return The loaded cursor
		 */
		virtual SystemCursor* LoadSystemCursor(SystemCursorType type) = 0;

		/**
		 * \brief Tell whether a SystemCursorType is supported
		 *
		 * \param systemCursorType The cursor type to test
		 *
		 * \return True if supported, false otherwise
		 */
		virtual bool IsSystemCursorSupported(SystemCursorType systemCursorType) const = 0;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
