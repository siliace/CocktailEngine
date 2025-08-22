#ifndef COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
#define COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP

#include <Windows.h>

#ifdef GetEnvironmentVariable
	#undef GetEnvironmentVariable
#endif

#include <Cocktail/Core/Application/Application.hpp>

namespace Ck::Main::Win32
{
	/**
     * \brief 
     */
    class Win32Application : public Application
    {
    public:

	    /**
         * \brief 
         * \param hInstance 
         * \param hPrevInstance 
         * \param pCmdLine 
         * \param nCmdShow 
         */
        Win32Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

		/**
		 * \brief
		 * \param exitCode
		 * \param force
		 * \param callSite
		 */
		void Exit(unsigned exitCode, bool force, std::string_view callSite) override;

		/**
		 * \brief
		 * \return
		 */
		const Array<std::string>& GetArgv() const override;

	    /**
	     * \brief 
	     * \param name 
	     * \return 
	     */
	    std::string GetEnvironmentVariable(std::string_view name) override;

	    /**
	     * \brief 
	     * \return 
	     */
	    bool IsDebuggerPresent() const override;

    private:

        Array<std::string> mArgv;
    };
}

#endif // COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
