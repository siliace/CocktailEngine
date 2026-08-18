#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPE_HPP

#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/System/Process/Win32/PipeReader.hpp>
#include <CocktailEngine/Core/System/Process/Win32/PipeWriter.hpp>

namespace Ck::Detail::Win32
{
    class Pipe
    {
    public:

        Pipe();

        PipeReader& GetReader() const;
        PipeWriter& GetWriter() const;

    private:

        UniquePtr<PipeReader> mReader;
        UniquePtr<PipeWriter> mWriter;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPE_HPP
