#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPEREADER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPEREADER_HPP

#include <CocktailEngine/Core/IO/Input/Reader/Reader.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    class PipeReader : public Reader<>
    {
    public:

        PipeReader(HANDLE handle);

        ~PipeReader() override;

        SizeType Read(CharType* text, SizeType length) override;

        bool HasCursor() const override;

        Uint64 Seek(Uint64 position) override;

        void Rewind() override;

        Uint64 Tell() const override;

        SizeType GetSize() const override;

        bool IsEof() const override;

        HANDLE GetSystemHandle() const;

    private:

        HANDLE mHandle;
        bool mEof;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PIPEREADER_HPP
