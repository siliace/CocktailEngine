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
		void Exit(unsigned exitCode, bool force, StringView callSite) override;

		/**
		 * \brief
		 * \return
		 */
		const Array<String>& GetArgv() const override;

	    /**
	     * \brief 
	     * \param name 
	     * \return 
	     */
		Optional<String> GetEnvironmentVariable(StringView name) override;

	    /**
	     * \brief 
	     * \return 
	     */
	    bool IsDebuggerPresent() const override;

    private:

        Array<String> mArgv;
    };
}

#endif // COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
