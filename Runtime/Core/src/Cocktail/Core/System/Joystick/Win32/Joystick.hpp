#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICK_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICK_HPP

#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Joystick/Joystick.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class Joystick : public Ck::Joystick, public Observable
	{
	public:

		/**
		 * \brief 
		 * \param index 
		 */
		explicit Joystick(unsigned int index);

		bool IsPlugged() const override;
		void SetWindowCapture(const Window& window, const Duration& pollingFrequency, unsigned axisThreshold) override;
		void ReleaseWindowCapture() override;
		bool IsButtonPressed(unsigned button) const override;
		unsigned GetAxisPosition(JoystickAxis axis) const override;
		Flags<JoystickPovPosition> GetPovPosition() const override;
		Signal<JoystickAxisEvent>& OnAxisEvent() override;
		Signal<JoystickAxisEvent>& OnAxisEvent(JoystickAxis axis) override;
		Signal<JoystickButtonEvent>& OnButtonEvent() override;
		Signal<JoystickButtonEvent>& OnButtonEvent(unsigned int button) override;
		Signal<JoystickButtonEvent>& OnButtonEvent(unsigned int button, bool pressed) override;

		const std::string& GetName() const override;

		const JoystickCapabilities& GetCapabilities() const override;

	private:

		/**
		 * \brief 
		 * \param index 
		 * \param capabilities 
		 * \param name 
		 */
		static void GetCapabilitiesAndName(UINT index, JoystickCapabilities& capabilities, std::string& name);

		/**
		 * \brief 
		 * \param flags 
		 * \return 
		 */
		JOYINFOEX GetInternalState(DWORD flags) const;

		unsigned int mIndex;
		std::string mName;
		JoystickCapabilities mCapabilities;
		HWND mCaptureHandle;
		Signal<JoystickAxisEvent> mOnAxesEvent;
		EnumMap<JoystickAxis, Signal<JoystickAxisEvent>> mOnAxisEvent;
		Signal<JoystickButtonEvent> mOnButtonsEvent;
		Signal<JoystickButtonEvent> mOnButtonEvent[Joystick::ButtonCount];
		Signal<JoystickButtonEvent> mOnButtonPressedEvent[Joystick::ButtonCount];
		Signal<JoystickButtonEvent> mOnButtonReleasedEvent[Joystick::ButtonCount];
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICK_HPP
