#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICK_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICK_HPP

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Joystick/JoystickCapabilities.hpp>
#include <Cocktail/Core/System/Joystick/JoystickEvent.hpp>
#include <Cocktail/Core/System/Joystick/JoystickManager.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
	class Window;

	/**
	 * \brief 
	 */
	class Joystick : public ServiceFacade<JoystickManager>
	{
	public:

		/**
		 * \brief
		 */
		static constexpr unsigned int ButtonCount = 32;

		/**
		 * \brief 
		 */
		static constexpr unsigned int AxisCount = Enum<JoystickAxis>::ValueCount;

		/**
		 * \brief Get the number of joystick plugged-in
		 * \return The number of joystick
		 */
		static unsigned int GetDeviceCount();

		/**
		 * \brief Get a joystick from its index
		 * \param index The index of the joystick to get
		 * \return The joystick it its exists, nullptr otherwise
		 */
		static Joystick* GetDevice(unsigned int index);

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool IsPlugged() const = 0;

		/**
		 * \brief 
		 * \param window
		 * \param pollingFrequency
		 * \param axisThreshold 
		 */
		virtual void SetWindowCapture(const Window& window, const Duration& pollingFrequency, unsigned int axisThreshold) = 0;

		/**
		 * \brief 
		 */
		virtual void ReleaseWindowCapture() = 0;

		/**
		 * \brief 
		 * \param button 
		 * \return 
		 */
		virtual bool IsButtonPressed(unsigned int button) const = 0;

		/**
		 * \brief 
		 * \param axis 
		 * \return 
		 */
		virtual unsigned int GetAxisPosition(JoystickAxis axis) const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Flags<JoystickPovPosition> GetPovPosition() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Signal<JoystickAxisEvent>& OnAxisEvent() = 0;

		/**
		 * \brief 
		 * \param axis 
		 * \return 
		 */
		virtual Signal<JoystickAxisEvent>& OnAxisEvent(JoystickAxis axis) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Signal<JoystickButtonEvent>& OnButtonEvent() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<JoystickButtonEvent>& OnButtonEvent(unsigned int button) = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<JoystickButtonEvent>& OnButtonEvent(unsigned int button, bool pressed) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const String& GetName() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const JoystickCapabilities& GetCapabilities() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICK_HPP
