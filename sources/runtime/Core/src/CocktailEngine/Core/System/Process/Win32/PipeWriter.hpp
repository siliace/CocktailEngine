#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPEWRITER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPEWRITER_HPP

#include <CocktailEngine/Core/IO/Output/Writer/Writer.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    class PipeWriter : public Writer<>
    {
    public:

        PipeWriter(HANDLE handle);
        ~PipeWriter() override;
        void Write(const CharType* text, SizeType length) override;
        void Flush() override;

        HANDLE GetSystemHandle() const;

    private:

        HANDLE mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPEWRITER_HPP
