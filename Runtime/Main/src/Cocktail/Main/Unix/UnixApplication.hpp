#ifndef COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
#define COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP

#include <Cocktail/Core/Application/Application.hpp>

namespace Ck::Main::Unix
{
    class UnixApplication : public Application
    {
    public:

        UnixApplication(int argc, char** argv);

	    /**
	     * \brief 
	     * \return 
	     */
	    const std::vector<std::string>& GetArgv() const override;

    private:

        std::vector<std::string> mArgv;
    };
}

#endif // COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
