#ifndef IMGUI_IMGUIINTEGRATION_IMUI_HPP
#define IMGUI_IMGUIINTEGRATION_IMUI_HPP

#include <imgui.h>

#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <CocktailEngine/Core/String.hpp>

#include <ImGui/ImGuiIntegration/Export.hpp>

#define CK_TEXT_TO_IMGUI(__Text) ::Ck::ImUi::TextToImGui(__Text).Get()
#define CK_TEXT_TO_IMGUI(__Text) ::Ck::ImUi::TextToImGui(__Text).Get()

namespace Ck::ImUi
{
    class IMGUI_IMGUIINTEGRATION_API TextToImGui
    {
    public:

        explicit TextToImGui(StringView stringView);

        const char* Get() const;

    private:

        Array<Utf8Char, LinearAllocator<1024>> mBuffer;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_IMUI_HPP
