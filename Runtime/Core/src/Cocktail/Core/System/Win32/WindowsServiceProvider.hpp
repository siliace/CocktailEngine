#ifndef COCKTAIL_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

CK_DECLARE_LOG_CATEGORY(Win32LogCategory, Ck::LogLevel::Info)

namespace Ck::Detail::Win32
{
    class WindowsServiceProvider : public ServiceProvider
    {
    public:

	    /**
	     * \brief 
	     * \param application 
	     */
	    explicit WindowsServiceProvider(Application* application);

    protected:

	    /**
	     * \brief 
	     * \param application 
	     */
	    void DoRegister(Application* application) override;

	    /**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(Application* application) override;

    private:

        HINSTANCE mInstanceHandle;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP
