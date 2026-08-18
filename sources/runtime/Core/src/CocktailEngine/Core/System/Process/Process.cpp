#include <CocktailEngine/Core/System/Process/Process.hpp>
#include <CocktailEngine/Core/System/Process/ProcessImpl.hpp>

namespace Ck
{
    UniquePtr<Process> Process::Create(const String& commandLine, ProcessIOStream streamMode, const HashMap<String, String>& environment, const Path& workingDirectory)
    {
        return MakeUnique<Detail::ProcessImpl>(commandLine, streamMode, environment, workingDirectory);
    }
}
