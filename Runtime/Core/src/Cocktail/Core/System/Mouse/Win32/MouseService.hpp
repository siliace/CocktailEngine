#ifndef COCKTAIL_CORE_SYSTEM_MOUSE_WIN32_MOUSESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MOUSE_WIN32_MOUSESERVICE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Core/System/Mouse/MouseService.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class MouseService : public Inherit<MouseService, Object, Ck::MouseService>
	{
	public:

		bool IsCursorVisible() const override;

		void SetCursorVisible(bool visible) override;

		Extent2D<int> GetCursorPosition() const override;

		void SetCursorPosition(const Extent2D<int>& position) override;

		bool IsPressedButton(MouseButton button) const override;

		Signal<MouseMovedEvent>& OnMouseMovedEvent() override;

		Signal<MouseButtonEvent>& OnMouseButtonEvent() override;

		Signal<MouseWheelEvent>& OnMouseWheelEvent() override;

	private:

		Signal<MouseMovedEvent> mOnMouseMovedEvent;
		Signal<MouseButtonEvent> mOnMouseButtonEvent;
		Signal<MouseWheelEvent> mOnMouseWheelEvent;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MOUSE_WIN32_MOUSESERVICE_HPP
