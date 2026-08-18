#ifndef COCKTAILENGINE_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP

#include <CocktailEngine/Core/Application/ServiceProvider.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	COCKTAIL_DECLARE_LOG_CATEGORY(Win32LogCategory, LogLevel::Info);

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

#endif // COCKTAILENGINE_CORE_SYSTEM_WIN32_WINDOWSSERVICEPROVIDER_HPP
