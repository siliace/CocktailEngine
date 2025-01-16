#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOW_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOW_HPP

#include <Cocktail/Core/Image/Image.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/System/Window/WindowEvent.hpp>

namespace Ck
{
    class Cursor;

    /**
     * \brief 
     */
    class Window : public Implements<Window, Interface>, public Observable
    {
    public:

        /**
         * \brief Process incoming events from the system
         * \return True if window is still opended after processed events, false otherwise
         */
        virtual bool PollEvents() = 0;

        /**
         * \brief
         */
        virtual void Update() = 0;

		/**
		 * \brief
		 * \param visible
		 */
		virtual void SetVisible(bool visible) = 0;

        /**
    	 * \brief 
    	 * \return 
    	 */
        virtual bool HasFocus() = 0;

        /**
         * \brief 
         */
        virtual void Minimize() = 0;

        /**
    	 * \brief 
    	 * \return 
    	 */
    	virtual bool IsMinimized() const = 0;

        /**
         * \brief 
         */
        virtual void Maximize() = 0;

        /**
    	 * \brief 
    	 * \return 
    	 */
    	virtual bool IsMaximized() const = 0;

        /**
         * \brief
         * \return
         */
        virtual Ref<Cursor> GetCursor() const = 0;

        /**
         * \brief
         * \param cursor
         */
        virtual void SetCursor(Ref<Cursor> cursor) = 0;

        virtual bool IsCursorVisible() const = 0;

        /**
         * \brief Change cursor visibility in the area of the window
         * \param visible 
         */
        virtual void SetCursorVisible(bool visible) = 0;

        /**
         * \brief Enable or disable the capture of the cursor inside the window
         * \param capture The state of the capture
         */
        virtual void EnableCaptureCursor(bool capture) = 0;

        /**
         * \brief 
         * \param image 
         */
        virtual void SetIcon(const Image* image) = 0;

        /**
         * \brief
         * \return
         */
        virtual std::string GetTitle() const = 0;

        /**
         * \brief
         * \param title
         */
        virtual void SetTitle(const std::string& title) = 0;

        /**
         * \brief
         * \return
         */
        virtual Extent2D<unsigned int> GetSize() const = 0;

        /**
         * \brief
         * \param size
         */
        virtual void SetSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief
         * \return
         */
        virtual Extent2D<int> GetPosition() const = 0;

        /**
         * \brief
         * \param position
         */
        virtual void SetPosition(const Extent2D<int>& position) = 0;

        /**
         * \brief
         * \return
         */
        virtual Extent2D<unsigned int> GetMinSize() const = 0;

        /**
         * \brief
         * \param size
         */
        virtual void SetMinSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief
         * \return
         */
        virtual Extent2D<unsigned int> GetMaxSize() const = 0;

        /**
         * \brief
         * \param size
         */
        virtual void SetMaxSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowCloseEvent>& OnCloseEvent() = 0;

        /**
         * \brief 
         * \return 
         */
        virtual Signal<WindowRedrawEvent>& OnRedrawEvent() = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowFocusEvent>& OnFocusEvent() = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowMovedEvent>& OnMovedEvent() = 0;
        
        /**
         * \brief
         * \return
         */
        virtual Signal<WindowResizedEvent>& OnResizedEvent() = 0;

        /**
         * \brief 
         * \return 
         */
        virtual Signal<WindowTextEvent>& OnTextEvent() = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowKeyboardEvent>& OnKeyboardEvent() = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowMouseMoveEvent>& OnMouseMovedEvent() = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowMouseButtonEvent>& OnMouseButtonEvent() = 0;

        /**
         * \brief
         * \return
         */
        virtual Signal<WindowMouseWheelEvent>& OnMouseWheelEvent() = 0;

        /**
         * \brief
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WINDOW_HPP