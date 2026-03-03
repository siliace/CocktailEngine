#ifndef COCKTAIL_SPONZA_IMGUI_IMUI_HPP
#define COCKTAIL_SPONZA_IMGUI_IMUI_HPP

#include <imgui.h>

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>

#define CK_TEXT_TO_IMGUI(__Text) ::Ck::ImUi::TextToImGui(__Text).Get()
#define CK_TEXT_TO_IMGUI(__Text) ::Ck::ImUi::TextToImGui(__Text).Get()

namespace Ck::ImUi
{
    class TextToImGui
    {
    public:

        explicit TextToImGui(StringView stringView);

        const char* Get() const;

    private:

        Array<Utf8Char, LinearAllocator<1024>> mBuffer;
    };
}

#endif // COCKTAIL_SPONZA_IMGUI_IMUI_HPP
