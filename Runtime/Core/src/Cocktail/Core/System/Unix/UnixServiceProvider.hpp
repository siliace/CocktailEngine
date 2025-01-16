#ifndef COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck::Detail::Unix
{
    class UnixServiceProvider : public ServiceProvider
    {
    public:

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
