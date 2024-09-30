#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP

#include <Cocktail/Core/System/Window/ImageCursor.hpp>
#include <Cocktail/Core/System/Window/SystemCursor.hpp>
#include <Cocktail/Core/System/Window/CursorCreateInfo.hpp>
#include <Cocktail/Core/System/Window/SystemCursorType.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>
#include <Cocktail/Core/System/Window/WindowCreateInfo.hpp>
#include <Cocktail/Core/System/Window/WindowEventQueue.hpp>

namespace Ck
{
	/**
	 * \brief Base interface exposing Curosr, Window and WindowEventQueue creation
	 */
	class WindowFactory : public Implements<WindowFactory, Interface>
	{
	public:

		/**
		 * \brief Create a new Cursor
		 * \param createInfo The structure describing the Cursor to create
		 * \return The created Cursor
		 */
		virtual Ref<ImageCursor> CreateCursor(const CursorCreateInfo& createInfo) = 0;

		/**
		 * \brief Create a new Window
		 * \param createInfo The structure describing the Window to create
		 * \return The created Window
		 */
		virtual Ref<Window> CreateWindow(const WindowCreateInfo& createInfo) = 0;

		/**
		* \brief Create a new WindowEventQueue
		* \return The created WindowEventQueue
		*/
		virtual Ref<WindowEventQueue> CreateWindowEventQueue() = 0;

		/**
		 * \brief Load a system's Cursor
		 * \param type The type of Cursor to load
		 * \return The loaded cursor
		 */
		virtual Ref<SystemCursor> LoadSystemCursor(SystemCursorType type) = 0;

		/**
		 * \brief Get the WindowEventQueue of the main thread of the application
		 * \return The main WindowEventQueue
		 */
		virtual WindowEventQueue* GetMainEventQueue() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWFACTORY_HPP
