#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOW_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOW_HPP

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
         * \brief
         * \return
         */
        virtual bool PollEvents() = 0;

        /**
         * \brief
         */
        virtual void Update() = 0;

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

        /**
         * \brief
         * \return
         */
        virtual std::string GetTitle() = 0;

        /**
         * \brief
         * \param title
         */
        virtual void SetTitle(const std::string& title) = 0;

        /**
         * \brief
         * \return
         */
        virtual Extent2D<unsigned int> GetSize() = 0;

        /**
         * \brief
         * \param size
         */
        virtual void SetSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief
         * \return
         */
        virtual Extent2D<int> GetPosition() = 0;

        /**
         * \brief
         * \param position
         */
        virtual void SetPosition(const Extent2D<int>& position) = 0;

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