#include <Cocktail/Main/Unix/UnixApplication.hpp>

namespace Ck::Main::Unix
{
    UnixApplication::UnixApplication(int argc, char** argv)
    {
        mArgv.Reserve(argc - 1);
        for (int i = 1; i < argc; i++)
            mArgv.Emplace(argv[i]);
    }

    const Array<std::string>& UnixApplication::GetArgv() const
    {
        return mArgv;
    }

    bool UnixApplication::IsDebuggerPresent() const
    {
        return false;
    }
}
