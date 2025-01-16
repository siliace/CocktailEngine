#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/System/Monitor/Xlib/MonitorService.hpp>
#include <Cocktail/Core/System/Mouse/Xlib/MouseService.hpp>
#include <Cocktail/Core/System/Keyboard/Xlib/KeyboardService.hpp>
#include <Cocktail/Core/System/Window/Xlib/AtomManager.hpp>
#include <Cocktail/Core/System/Window/Xlib/WindowFactory.hpp>
#include <Cocktail/Core/System/Unix/Xlib/XlibServiceProvider.hpp>

namespace Ck::Detail::Xlib
{
    XlibServiceProvider::XlibServiceProvider(Application* application) :
		ServiceProvider(application)
	{
    	XInitThreads();
		mDisplay = ::XOpenDisplay(nullptr);
		XSetErrorHandler([](::Display* display, XErrorEvent* errorEvent) {
			return 0;
		});
	}

	XlibServiceProvider::~XlibServiceProvider()
	{
		XSetErrorHandler(mPreviousErrorHandler);
		XCloseDisplay(mDisplay);
	    XFreeThreads();
	}    

    void XlibServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<Xlib::AtomManager>([&]() {
			return std::make_unique<AtomManager>(mDisplay);
		});
		
		int version = 0;
		if (XQueryExtension(mDisplay, "RANDR", &version, &version, &version))
		{
			application->Singleton<Ck::MonitorService>([&]() -> std::unique_ptr<Ck::MonitorService> {
				return std::make_unique<MonitorService>(mDisplay);
			});
		}	
		else
		{
			application->Invoke([](LogManager* log) {
				log->Warning("X11 extension RANDR is not supported, MonitorService will be disabled");
			});
		}

		application->Singleton<Ck::MouseService>([&]() -> std::unique_ptr<Ck::MouseService> {
			return std::make_unique<MouseService>(mDisplay, application);
		});
		
        application->Singleton<Ck::KeyboardService>([&]() -> std::unique_ptr<Ck::KeyboardService> {
			return std::make_unique<KeyboardService>(mDisplay, application);
		});
		
        application->Singleton<Ck::WindowFactory>([&]() -> std::unique_ptr<Ck::WindowFactory> {
			return std::make_unique<WindowFactory>(mDisplay);
		});
	}
}
