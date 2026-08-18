#ifndef COCKTAILENGINE_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP

#include <CocktailEngine/Core/Application/ServiceProvider.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>

namespace Ck::Detail::Unix
{
	COCKTAIL_DECLARE_LOG_CATEGORY(UnixLogCategory, LogLevel::Info);

	class UnixServiceProvider : public ServiceProvider
    {
    public:

	    /**
	     * \brief Constructor
	     * \param application The application where this service provider will be registered
	     */
	    explicit UnixServiceProvider(Application* application);

    protected:

	    /**
	     * \brief 
	     * \param application 
	     */
	    void DoRegister(Application* application) override;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
