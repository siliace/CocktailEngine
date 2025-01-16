#include <Cocktail/Main/Unix/UnixApplication.hpp>

namespace Ck::Main::Unix
{
    UnixApplication::UnixApplication(int argc, char** argv)
    {
        mArgv.reserve(argc - 1);
        for (int i = 1; i < argc; i++)
            mArgv.emplace_back(argv[i]);
    }

    const std::vector<std::string>& UnixApplication::GetArgv() const
    {
        return mArgv;
    }
}
