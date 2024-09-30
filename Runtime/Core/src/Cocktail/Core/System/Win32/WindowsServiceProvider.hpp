#ifndef COCKTAIL_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    class WindowsServiceProvider : public Extends<WindowsServiceProvider, ServiceProvider>
    {
    public:

	    /**
	     * \brief 
	     * \param application 
	     */
	    explicit WindowsServiceProvider(const Ref<Application>& application);

    protected:

	    /**
	     * \brief 
	     * \param application 
	     */
	    void DoRegister(const Ref<Application>& application) override;

    private:

        HINSTANCE mInstanceHandle;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP
