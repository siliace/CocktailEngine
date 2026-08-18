#include <CocktailEngine/Core/Application/Application.hpp>

#include <ImGui/ImGuiIntegration/ImUi/ImUi.hpp>

namespace Ck::ImUi
{
    TextToImGui::TextToImGui(StringView stringView)
    {
        if (!stringView.IsEmpty())
            mBuffer = Encoders::Convert<Encoders::Text, Encoders::Utf8, LinearAllocator<1024>>(stringView.GetData(), stringView.GetLength());
        mBuffer.Add(CK_UTF8_CHAR('\0'));
    }

    const char* TextToImGui::Get() const
    {
        return reinterpret_cast<const char*>(mBuffer.GetData());
    }
}
