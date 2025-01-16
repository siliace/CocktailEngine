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
    XlibServiceProvider::XlibServiceProvider(Ref<Application> application) :
		Extends<XlibServiceProvider, ServiceProvider>(std::move(application))
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

    void XlibServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<Xlib::AtomManager>([&]() {
			return AtomManager::New(mDisplay);
		});
		
		int version = 0;
		if (XQueryExtension(mDisplay, "RANDR", &version, &version, &version))
		{
			application->Singleton<Ck::MonitorService>([&]() -> Ref<Ck::MonitorService> {
				return MonitorService::New(mDisplay);
			});
		}	
		else
		{
			application->Invoke([](const Ref<LogManager>& log) {
				log->Warning("X11 extension RANDR is not supported, MonitorService will be disabled");
			});
		}

		application->Singleton<Ck::MouseService>([&]() -> Ref<Ck::MouseService> {
			return MouseService::New(mDisplay, application);
		});
		
        application->Singleton<Ck::KeyboardService>([&]() -> Ref<Ck::KeyboardService> {
			return KeyboardService::New(mDisplay, application);
		});
		
        application->Singleton<Ck::WindowFactory>([&]() -> Ref<Ck::WindowFactory> {
			return WindowFactory::New(mDisplay);
		});
	}
}
