#ifndef COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck::Detail::Unix
{
    class UnixServiceProvider : public Extends<UnixServiceProvider, ServiceProvider>
    {
    public:

        explicit UnixServiceProvider(const Ref<Application>& application);

    protected:

	    /**
	     * \brief 
	     * \param application 
	     */
	    void DoRegister(const Ref<Application>& application) override;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_UNIX_UNIXSERVICEPROVIDER_HPP
