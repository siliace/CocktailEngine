#ifndef COCKTAIL_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
	COCKTAIL_DECLARE_LOG_CATEGORY(XlibLogCategory, LogLevel::Info);

    class XlibServiceProvider : public ServiceProvider
	{
    public:

		/**
		 * \brief Constructor
	     * \param application The application where this service provider will be registered
		 */
		explicit XlibServiceProvider(Application* application);

        /**
         * \brief Destructor
         */
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
