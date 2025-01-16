#ifndef COCKTAIL_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
    class XlibServiceProvider : public ServiceProvider
	{
    public:

        explicit XlibServiceProvider(Application* application);

		~XlibServiceProvider() override;

    protected:

	    /**
	     * \brief 
	     * \param application 
	     */
	    void DoRegister(Application* application) override;

	private:

		::Display* mDisplay;
		::XErrorHandler mPreviousErrorHandler;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP
