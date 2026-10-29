#include <CocktailEngine/Core/GenericException.hpp>
#include <CocktailEngine/Core/System/Joystick/Win32/Joystick.hpp>
#include <CocktailEngine/Core/System/Window/Window.hpp>

namespace Ck::Detail::Win32
{
    class JoystickErrorCodeTranslator : public ErrorCodeTranslator<unsigned int>
    {
    public:

        String TranslateErrorCode(unsigned int errorCode) const override
        {
            switch (errorCode)
            {
                case JOYERR_NOERROR: return CK_TEXT("NoError");
                case JOYERR_PARMS: return CK_TEXT("InvalidParameter");
                case JOYERR_NOCANDO: return CK_TEXT("InvalidOperation");
                case JOYERR_UNPLUGGED: return CK_TEXT("JoystickUnplugged");
            }

            return String::Format(CK_TEXT("Unknown joystick error (%u)"), errorCode);
        }
    };

    using JoystickException = GenericException<unsigned int, JoystickErrorCodeTranslator>;

    Joystick::Joystick(unsigned int index) :
        mIndex(index),
        mCaptureHandle(nullptr)
    {
        GetCapabilitiesAndName(mIndex, mCapabilities, mName);

        Connect(mOnAxesEvent, [&](const JoystickAxisEvent& event) {
            OnAxisEvent(event.Axis).Emit(event);
        });

        Connect(mOnButtonsEvent, [&](const JoystickButtonEvent& event) {
            OnButtonEvent(event.Button).Emit(event);
        });

        for (unsigned int i = 0; i < Joystick::ButtonCount; i++)
        {
            Connect(mOnButtonEvent[i], [&](const JoystickButtonEvent& event) {
                OnButtonEvent(event.Button, event.Pressed).Emit(event);
            });
        }
    }

    bool Joystick::IsPlugged() const
    {
        JOYINFO joystickState;
        return joyGetPos(mIndex, &joystickState) == JOYERR_NOERROR;
    }

    void Joystick::SetWindowCapture(const Window& window, const Duration& pollingFrequency, unsigned int axisThreshold)
    {
        if (!IsPlugged())
            throw JoystickException(JOYERR_UNPLUGGED);

        if (mCaptureHandle)
            ReleaseWindowCapture();

        HWND captureHandle = static_cast<HWND>(window.GetSystemHandle());

        unsigned int period = static_cast<unsigned int>(pollingFrequency.As(TimeUnit::Milliseconds()).GetCount());
        MMRESULT result = joySetCapture(captureHandle, mIndex, period, TRUE);
        if (result != JOYERR_NOERROR)
            throw JoystickException(result);

        result = joySetThreshold(mIndex, axisThreshold);
        if (result != JOYERR_NOERROR)
            throw JoystickException(result);

        mCaptureHandle = captureHandle;
    }

    void Joystick::ReleaseWindowCapture()
    {
        if (!mCaptureHandle)
            return;

        if (!IsPlugged())
            throw JoystickException(JOYERR_UNPLUGGED);

        joyReleaseCapture(mIndex);
        mCaptureHandle = nullptr;
    }

    bool Joystick::IsButtonPressed(unsigned int button) const
    {
        return GetInternalState(JOY_RETURNBUTTONS).dwButtons & Bit(button);
    }

    unsigned int Joystick::GetAxisPosition(JoystickAxis axis) const
    {
        switch (axis)
        {
            case JoystickAxis::X: return GetInternalState(JOY_RETURNX).dwXpos;
            case JoystickAxis::Y: return GetInternalState(JOY_RETURNY).dwYpos;
            case JoystickAxis::Z: return GetInternalState(JOY_RETURNZ).dwZpos;
            case JoystickAxis::R: return GetInternalState(JOY_RETURNR).dwRpos;
            case JoystickAxis::U: return GetInternalState(JOY_RETURNU).dwUpos;
            case JoystickAxis::V: return GetInternalState(JOY_RETURNV).dwVpos;
        }

        return 0;
    }

    Flags<JoystickPovPosition> Joystick::GetPovPosition() const
    {
        Flags<JoystickPovPosition> position;
        JOYINFOEX joystickState = GetInternalState(JOY_RETURNPOV);

        if (joystickState.dwPOV != JOY_POVCENTERED)
        {
            if (joystickState.dwPOV > JOY_POVLEFT || joystickState.dwPOV < JOY_POVRIGHT)
                position |= JoystickPovPosition::Up;

            if (joystickState.dwPOV > JOY_POVFORWARD && joystickState.dwPOV < JOY_POVBACKWARD)
                position |= JoystickPovPosition::Right;

            if (joystickState.dwPOV > JOY_POVRIGHT && joystickState.dwPOV < JOY_POVLEFT)
                position |= JoystickPovPosition::Down;

            if (joystickState.dwPOV > JOY_POVBACKWARD)
                position |= JoystickPovPosition::Left;
        }

        return position;
    }

    Signal<JoystickAxisEvent>& Joystick::OnAxisEvent()
    {
        return mOnAxesEvent;
    }

    Signal<JoystickAxisEvent>& Joystick::OnAxisEvent(JoystickAxis axis)
    {
        return mOnAxisEvent[axis];
    }

    Signal<JoystickButtonEvent>& Joystick::OnButtonEvent()
    {
        return mOnButtonsEvent;
    }

    Signal<JoystickButtonEvent>& Joystick::OnButtonEvent(unsigned int button)
    {
        return mOnButtonEvent[button];
    }

    Signal<JoystickButtonEvent>& Joystick::OnButtonEvent(unsigned int button, bool pressed)
    {
        if (pressed)
            return mOnButtonPressedEvent[button];

        return mOnButtonReleasedEvent[button];
    }

    const String& Joystick::GetName() const
    {
        return mName;
    }

    const JoystickCapabilities& Joystick::GetCapabilities() const
    {
        return mCapabilities;
    }

    void Joystick::GetCapabilitiesAndName(UINT index, JoystickCapabilities& capabilities, String& name)
    {
        JOYCAPS2 infoCaps;
        ZeroMemory(&infoCaps, sizeof(JOYCAPS2));

        MMRESULT result = joyGetDevCaps(index, reinterpret_cast<JOYCAPS*>(&infoCaps), sizeof(JOYCAPS2));
        if (result != JOYERR_NOERROR)
            throw JoystickException(result);

        name = infoCaps.szPname;

        capabilities.Axes[JoystickAxis::X].Supported = true;
        capabilities.Axes[JoystickAxis::Y].Supported = true;
        capabilities.Axes[JoystickAxis::Z].Supported = infoCaps.wCaps & JOYCAPS_HASZ;
        capabilities.Axes[JoystickAxis::R].Supported = infoCaps.wCaps & JOYCAPS_HASR;
        capabilities.Axes[JoystickAxis::U].Supported = infoCaps.wCaps & JOYCAPS_HASU;
        capabilities.Axes[JoystickAxis::V].Supported = infoCaps.wCaps & JOYCAPS_HASV;

        capabilities.Axes[JoystickAxis::X].Min = infoCaps.wXmin;
        capabilities.Axes[JoystickAxis::Y].Min = infoCaps.wYmin;
        capabilities.Axes[JoystickAxis::Z].Min = infoCaps.wZmin;
        capabilities.Axes[JoystickAxis::R].Min = infoCaps.wRmin;
        capabilities.Axes[JoystickAxis::U].Min = infoCaps.wUmin;
        capabilities.Axes[JoystickAxis::V].Min = infoCaps.wVmin;

        capabilities.Axes[JoystickAxis::X].Max = infoCaps.wXmax;
        capabilities.Axes[JoystickAxis::Y].Max = infoCaps.wYmax;
        capabilities.Axes[JoystickAxis::Z].Max = infoCaps.wZmax;
        capabilities.Axes[JoystickAxis::R].Max = infoCaps.wRmax;
        capabilities.Axes[JoystickAxis::U].Max = infoCaps.wUmax;
        capabilities.Axes[JoystickAxis::V].Max = infoCaps.wVmax;

        capabilities.AxisCount = infoCaps.wNumAxes;
        capabilities.ButtonCount = infoCaps.wNumButtons;
        capabilities.SupportHatPov = infoCaps.wCaps & JOYCAPS_HASPOV;
    }

    JOYINFOEX Joystick::GetInternalState(DWORD flags) const
    {
        JOYINFOEX joystickState;
        ZeroMemory(&joystickState, sizeof(JOYINFOEX));

        joystickState.dwSize = sizeof(JOYINFOEX);
        joystickState.dwFlags = flags;

        MMRESULT result = joyGetPosEx(mIndex, &joystickState);
        if (result != JOYERR_NOERROR)
            throw JoystickException(result);

        return joystickState;
    }
}
