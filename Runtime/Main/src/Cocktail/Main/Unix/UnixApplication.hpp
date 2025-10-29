#ifndef COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
#define COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP

#include <Cocktail/Core/Application/Application.hpp>

namespace Ck::Main::Unix
{
    class UnixApplication : public Application
    {
    public:

	    /**
	     * \brief Constructor
	     * \param argc
	     * \param argv
	     */
	    UnixApplication(int argc, char** argv);

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

	    bool IsDebuggerPresent() const override;

	    Path GetExecutablePath() const override;

    private:

        Array<String> mArgv;
    };
}

#endif // COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
