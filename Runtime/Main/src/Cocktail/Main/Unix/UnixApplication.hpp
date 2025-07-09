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
	     * \return 
	     */
	    const Array<std::string>& GetArgv() const override;

	    bool IsDebuggerPresent() const override;

    private:

        Array<std::string> mArgv;
    };
}

#endif // COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
