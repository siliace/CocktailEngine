#include <Cocktail/Sponza/ImGuiServiceProvider.hpp>
#include <Cocktail/Sponza/ImGui/imgui.h>
#include <Cocktail/Sponza/ImGui/imgui_internal.h>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Clipboard/ClipboardService.hpp>
#include <Cocktail/Core/System/Window/WindowFactory.hpp>

using namespace Ck;

namespace
{
	const char* GetClipboardText(ImGuiContext* context)
	{
		ClipboardService* clipboard = static_cast<ClipboardService*>(context->PlatformIO.Platform_ClipboardUserData);

		context->ClipboardHandlerData.clear();
		std::size_t length = clipboard->GetSize();

		context->ClipboardHandlerData.resize(length);
		clipboard->Get(context->ClipboardHandlerData.Data, length);

		return context->ClipboardHandlerData.Data;
	}

	void SetClipboardText(ImGuiContext* context, const char* content)
	{
		ClipboardService* clipboard = static_cast<ClipboardService*>(context->PlatformIO.Platform_ClipboardUserData);
		clipboard->Set(content, StringUtils<AnsiChar, Uint32>::GetLength(content));
	}

	Optional<int> ToImguiButton(MouseButton button)
	{
		switch (button)
		{
		case MouseButton::Left: return Optional<int>::Of(ImGuiMouseButton_Left);
		case MouseButton::Middle: return Optional<int>::Of(ImGuiMouseButton_Middle);
		case MouseButton::Right: return Optional<int>::Of(ImGuiMouseButton_Right);
		}

		return Optional<int>::Empty();
	}

	Optional<ImGuiKey> ToImguiKey(KeyboardKey key)
	{
		switch (key)
		{
		case KeyboardKey::Backspace: return Optional<ImGuiKey>::Of(ImGuiKey_Backspace);
		case KeyboardKey::Tabulation: return Optional<ImGuiKey>::Of(ImGuiKey_Tab);
		case KeyboardKey::Enter: return Optional<ImGuiKey>::Of(ImGuiKey_Enter);
		case KeyboardKey::Pause: return Optional<ImGuiKey>::Of(ImGuiKey_Pause);
		case KeyboardKey::CapsLock: return Optional<ImGuiKey>::Of(ImGuiKey_CapsLock);
		case KeyboardKey::Escape: return Optional<ImGuiKey>::Of(ImGuiKey_Escape);
		case KeyboardKey::Space: return Optional<ImGuiKey>::Of(ImGuiKey_Space);
		case KeyboardKey::Prior: return Optional<ImGuiKey>::Of(ImGuiKey_PageDown);
		case KeyboardKey::Next: return Optional<ImGuiKey>::Of(ImGuiKey_PageUp);
		case KeyboardKey::End: return Optional<ImGuiKey>::Of(ImGuiKey_End);
		case KeyboardKey::Home: return Optional<ImGuiKey>::Of(ImGuiKey_Home);
		case KeyboardKey::Left: return Optional<ImGuiKey>::Of(ImGuiKey_LeftArrow);
		case KeyboardKey::Up: return Optional<ImGuiKey>::Of(ImGuiKey_UpArrow);
		case KeyboardKey::Right: return Optional<ImGuiKey>::Of(ImGuiKey_RightArrow);
		case KeyboardKey::Down: return Optional<ImGuiKey>::Of(ImGuiKey_DownArrow);
		case KeyboardKey::ScreenShot: return Optional<ImGuiKey>::Of(ImGuiKey_PrintScreen);
		case KeyboardKey::Insert: return Optional<ImGuiKey>::Of(ImGuiKey_Insert);
		case KeyboardKey::Delete: return Optional<ImGuiKey>::Of(ImGuiKey_Delete);
		case KeyboardKey::A: return Optional<ImGuiKey>::Of(ImGuiKey_A);
		case KeyboardKey::B: return Optional<ImGuiKey>::Of(ImGuiKey_B);
		case KeyboardKey::C: return Optional<ImGuiKey>::Of(ImGuiKey_C);
		case KeyboardKey::D: return Optional<ImGuiKey>::Of(ImGuiKey_D);
		case KeyboardKey::E: return Optional<ImGuiKey>::Of(ImGuiKey_E);
		case KeyboardKey::F: return Optional<ImGuiKey>::Of(ImGuiKey_F);
		case KeyboardKey::G: return Optional<ImGuiKey>::Of(ImGuiKey_G);
		case KeyboardKey::H: return Optional<ImGuiKey>::Of(ImGuiKey_H);
		case KeyboardKey::I: return Optional<ImGuiKey>::Of(ImGuiKey_I);
		case KeyboardKey::J: return Optional<ImGuiKey>::Of(ImGuiKey_J);
		case KeyboardKey::K: return Optional<ImGuiKey>::Of(ImGuiKey_K);
		case KeyboardKey::L: return Optional<ImGuiKey>::Of(ImGuiKey_L);
		case KeyboardKey::M: return Optional<ImGuiKey>::Of(ImGuiKey_M);
		case KeyboardKey::N: return Optional<ImGuiKey>::Of(ImGuiKey_N);
		case KeyboardKey::O: return Optional<ImGuiKey>::Of(ImGuiKey_O);
		case KeyboardKey::P: return Optional<ImGuiKey>::Of(ImGuiKey_P);
		case KeyboardKey::Q: return Optional<ImGuiKey>::Of(ImGuiKey_Q);
		case KeyboardKey::R: return Optional<ImGuiKey>::Of(ImGuiKey_R);
		case KeyboardKey::S: return Optional<ImGuiKey>::Of(ImGuiKey_S);
		case KeyboardKey::T: return Optional<ImGuiKey>::Of(ImGuiKey_T);
		case KeyboardKey::U: return Optional<ImGuiKey>::Of(ImGuiKey_U);
		case KeyboardKey::V: return Optional<ImGuiKey>::Of(ImGuiKey_V);
		case KeyboardKey::W: return Optional<ImGuiKey>::Of(ImGuiKey_W);
		case KeyboardKey::X: return Optional<ImGuiKey>::Of(ImGuiKey_X);
		case KeyboardKey::Y: return Optional<ImGuiKey>::Of(ImGuiKey_Y);
		case KeyboardKey::Z: return Optional<ImGuiKey>::Of(ImGuiKey_Z);
		case KeyboardKey::LeftSystem: return Optional<ImGuiKey>::Of(ImGuiKey_LeftSuper);
		case KeyboardKey::RightSystem: return Optional<ImGuiKey>::Of(ImGuiKey_RightSuper);
		case KeyboardKey::App: return Optional<ImGuiKey>::Of(ImGuiKey_Menu);
		case KeyboardKey::NumPad0: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad0);
		case KeyboardKey::NumPad1: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad1);
		case KeyboardKey::NumPad2: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad2);
		case KeyboardKey::NumPad3: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad3);
		case KeyboardKey::NumPad4: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad4);
		case KeyboardKey::NumPad5: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad5);
		case KeyboardKey::NumPad6: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad6);
		case KeyboardKey::NumPad7: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad7);
		case KeyboardKey::NumPad8: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad8);
		case KeyboardKey::NumPad9: return Optional<ImGuiKey>::Of(ImGuiKey_Keypad9);
		case KeyboardKey::Multiply: return Optional<ImGuiKey>::Of(ImGuiKey_KeypadMultiply);
		case KeyboardKey::Add: return Optional<ImGuiKey>::Of(ImGuiKey_KeypadAdd);
		case KeyboardKey::Separator: return Optional<ImGuiKey>::Of(ImGuiKey_Period);
		case KeyboardKey::Subtract: return Optional<ImGuiKey>::Of(ImGuiKey_KeypadSubtract);
		case KeyboardKey::Decimal: return Optional<ImGuiKey>::Of(ImGuiKey_KeypadDecimal);
		case KeyboardKey::Divide: return Optional<ImGuiKey>::Of(ImGuiKey_KeypadDivide);
		case KeyboardKey::F1: return Optional<ImGuiKey>::Of(ImGuiKey_F1);
		case KeyboardKey::F2: return Optional<ImGuiKey>::Of(ImGuiKey_F2);
		case KeyboardKey::F3: return Optional<ImGuiKey>::Of(ImGuiKey_F3);
		case KeyboardKey::F4: return Optional<ImGuiKey>::Of(ImGuiKey_F4);
		case KeyboardKey::F5: return Optional<ImGuiKey>::Of(ImGuiKey_F5);
		case KeyboardKey::F6: return Optional<ImGuiKey>::Of(ImGuiKey_F6);
		case KeyboardKey::F7: return Optional<ImGuiKey>::Of(ImGuiKey_F7);
		case KeyboardKey::F8: return Optional<ImGuiKey>::Of(ImGuiKey_F8);
		case KeyboardKey::F9: return Optional<ImGuiKey>::Of(ImGuiKey_F9);
		case KeyboardKey::F10: return Optional<ImGuiKey>::Of(ImGuiKey_F10);
		case KeyboardKey::F11: return Optional<ImGuiKey>::Of(ImGuiKey_F11);
		case KeyboardKey::F12: return Optional<ImGuiKey>::Of(ImGuiKey_F12);
		case KeyboardKey::F13: return Optional<ImGuiKey>::Of(ImGuiKey_F13);
		case KeyboardKey::F14: return Optional<ImGuiKey>::Of(ImGuiKey_F14);
		case KeyboardKey::F15: return Optional<ImGuiKey>::Of(ImGuiKey_F15);
		case KeyboardKey::F16: return Optional<ImGuiKey>::Of(ImGuiKey_F16);
		case KeyboardKey::F17: return Optional<ImGuiKey>::Of(ImGuiKey_F17);
		case KeyboardKey::F18: return Optional<ImGuiKey>::Of(ImGuiKey_F18);
		case KeyboardKey::F19: return Optional<ImGuiKey>::Of(ImGuiKey_F19);
		case KeyboardKey::F20: return Optional<ImGuiKey>::Of(ImGuiKey_F20);
		case KeyboardKey::F21: return Optional<ImGuiKey>::Of(ImGuiKey_F21);
		case KeyboardKey::F22: return Optional<ImGuiKey>::Of(ImGuiKey_F22);
		case KeyboardKey::F23: return Optional<ImGuiKey>::Of(ImGuiKey_F23);
		case KeyboardKey::F24: return Optional<ImGuiKey>::Of(ImGuiKey_F24);
		case KeyboardKey::NumLock: return Optional<ImGuiKey>::Of(ImGuiKey_NumLock);
		case KeyboardKey::ScrollLock: return Optional<ImGuiKey>::Of(ImGuiKey_ScrollLock);
		case KeyboardKey::LeftShift: return Optional<ImGuiKey>::Of(ImGuiKey_LeftShift);
		case KeyboardKey::RightShift: return Optional<ImGuiKey>::Of(ImGuiKey_RightShift);
		case KeyboardKey::LeftControl: return Optional<ImGuiKey>::Of(ImGuiKey_LeftCtrl);
		case KeyboardKey::RightControl: return Optional<ImGuiKey>::Of(ImGuiKey_RightCtrl);
		case KeyboardKey::LeftAlt: return Optional<ImGuiKey>::Of(ImGuiKey_LeftAlt);
		case KeyboardKey::RightAlt: return Optional<ImGuiKey>::Of(ImGuiKey_RightAlt);
		}

		return Optional<ImGuiKey>::Empty();
	}
}

void ImGuiServiceProvider::DoBoot(Application* application) 
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	WindowFactory* windowFactory = application->Resolve<WindowFactory>();
	Connect(windowFactory->OnWindowCreated(), [&](std::shared_ptr<Window> window) {
		if (mEventWindow)
			return;

		ConnectWindowEvents(*window);

		Extent2D<unsigned int> windowSize = window->GetSize();
		io.DisplaySize = ImVec2(static_cast<float>(windowSize.Width), static_cast<float>(windowSize.Height));

		mEventWindow = window;
	});

	ImGuiPlatformIO& platformIo = ImGui::GetPlatformIO();
	platformIo.Platform_GetClipboardTextFn = GetClipboardText;
	platformIo.Platform_SetClipboardTextFn = SetClipboardText;
	platformIo.Platform_ClipboardUserData = application->Resolve<ClipboardService>();
}

void ImGuiServiceProvider::ConnectWindowEvents(Window& window)
{
	ImGuiIO& io = ImGui::GetIO();
	Connect(window.OnCloseEvent(), [&](const WindowCloseEvent& event) {
		mEventWindow = nullptr;
	});

	Connect(window.OnResizedEvent(), [&](const WindowResizedEvent& event) {
		io.DisplaySize = ImVec2(event.Size.Width, event.Size.Height);
	});

	Connect(window.OnMouseMovedEvent(), [&](const WindowMouseMoveEvent& event) {
		io.AddMousePosEvent(event.Position.Width, event.Position.Height);
	});

	Connect(window.OnFocusEvent(), [&](const WindowFocusEvent& event) {
		io.AddFocusEvent(event.HasFocus);
	});

	Connect(window.OnMouseButtonEvent(), [&](const WindowMouseButtonEvent& event) {
		switch (event.Type)
		{
		case MouseButtonEvent::ClickType::Pressed:
			ToImguiButton(event.Button).Then([&](int key) {
				io.AddMouseButtonEvent(key, true);
			});
			break;

		case MouseButtonEvent::ClickType::Released:
			ToImguiButton(event.Button).Then([&](int key) {
				io.AddMouseButtonEvent(key, false);
			});
			break;
		}
	});

	Connect(window.OnMouseWheelEvent(), [&](const WindowMouseWheelEvent& event) {
		switch (event.Wheel)
		{
		case MouseWheel::Vertical:
			io.AddMouseWheelEvent(0.f, event.Delta);
			break;

		case MouseWheel::Horizontal:
			io.AddMouseWheelEvent(event.Delta, 0.f);
			break;
		}
	});

	Connect(window.OnKeyboardEvent(), [&](const WindowKeyboardEvent& event) {
		ToImguiKey(event.Key).Then([&](ImGuiKey key) {
			io.AddKeyEvent(key, event.Pressed);
		});
	});

	Connect(window.OnTextEvent(), [&](const WindowTextEvent& event) {
		io.AddInputCharacter(event.Unicode);
	});
}

COCKTAIL_REGISTER_SERVICE_PROVIDER(ImGuiServiceProvider);
