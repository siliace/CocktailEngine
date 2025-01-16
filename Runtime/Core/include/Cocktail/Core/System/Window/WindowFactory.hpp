#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP

#include <Cocktail/Core/System/Window/ImageCursor.hpp>
#include <Cocktail/Core/System/Window/SystemCursor.hpp>
#include <Cocktail/Core/System/Window/SystemCursorType.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>
#include <Cocktail/Core/System/Window/WindowCreateInfo.hpp>

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
		 * \param image
		 * \param hotspot
		 * \return The created Cursor
		 */
		virtual Ref<ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) = 0;

		/**
		 * \brief Create a new Window
		 * \param createInfo The structure describing the Window to create
		 * \return The created Window
		 */
		virtual Ref<Window> CreateWindow(const WindowCreateInfo& createInfo) = 0;

		/**
		 * \brief Load a system's Cursor
		 * \param type The type of Cursor to load
		 * \return The loaded cursor
		 */
		virtual Ref<SystemCursor> LoadSystemCursor(SystemCursorType type) = 0;

		/**
		 * \brief Tell whether a SystemCursorType is supported
		 * \param systemCursorType The cursor type to test
		 * \return True if supported, false otherwise
		 */
		virtual bool IsSystemCursorSupported(SystemCursorType systemCursorType) const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Signal<Ref<Window>>& OnWindowCreated() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
