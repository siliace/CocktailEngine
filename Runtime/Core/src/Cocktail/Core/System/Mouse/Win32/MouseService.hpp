#ifndef COCKTAIL_CORE_SYSTEM_MOUSE_WIN32_MOUSESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MOUSE_WIN32_MOUSESERVICE_HPP

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Mouse/MouseService.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class MouseService : public Ck::MouseService
	{
	public:

		explicit MouseService(Application* application);

		bool IsCursorVisible() const override;

		void SetCursorVisible(bool visible) override;

		Extent2D<int> GetCursorPosition() const override;

		void SetCursorPosition(const Extent2D<int>& position) override;

		bool IsPressedButton(MouseButton button) const override;

		Signal<MouseMovedEvent>& OnMouseMovedEvent() override;

		Signal<MouseButtonEvent>& OnMouseButtonEvent() override;

		Signal<MouseButtonEvent>& OnMouseButtonPressedEvent(MouseButton button) override;

		Signal<MouseButtonEvent>& OnMouseButtonReleasedEvent(MouseButton button) override;

		Signal<MouseWheelEvent>& OnMouseWheelEvent() override;

		Signal<MouseWheelEvent>& OnMouseWheelEvent(MouseWheel wheel) override;

	private:

		Signal<MouseMovedEvent> mOnMouseMovedEvent;
		Signal<MouseButtonEvent> mOnMouseButtonEvent;
		std::unordered_map<MouseButton, Signal<MouseButtonEvent>> mOnButtonPressedEvents;
		std::unordered_map<MouseButton, Signal<MouseButtonEvent>> mOnButtonReleasedEvents;
		Signal<MouseWheelEvent> mOnMouseWheelEvent;
		std::unordered_map<MouseWheel, Signal<MouseWheelEvent>> mOnMouseWheelEvents;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MOUSE_WIN32_MOUSESERVICE_HPP
