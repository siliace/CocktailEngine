#ifndef COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>

namespace Ck::Detail::Unix
{
	CK_DECLARE_LOG_CATEGORY(UnixLogCategory, LogLevel::Info);

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

#endif // COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
