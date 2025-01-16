#ifndef COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
#define COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP

#include <Windows.h>

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
	     * \return 
	     */
	    const std::vector<std::string>& GetArgv() const override;

    private:

        std::vector<std::string> mArgv;
    };
}

#endif // COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
