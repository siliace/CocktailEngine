#ifndef COCKTAILENGINE_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP

#include <CocktailEngine/Core/Application/ServiceProvider.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>
#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_XLIB_XLIBSERVICEPROVIDER_HPP
