#include <Cocktail/Sponza/ImUi/ImUi.hpp>

namespace Ck::ImUi
{
    TextToImGui::TextToImGui(StringView stringView)
    {
        mBuffer = Encoders::Convert<Encoders::Text, Encoders::Utf8, LinearAllocator<1024>>(stringView.GetData(), stringView.GetLength());
        mBuffer.Add(CK_UTF8_CHAR('\0'));
    }

    const char* TextToImGui::Get() const
    {
        return reinterpret_cast<const char*>(mBuffer.GetData());
    }
}
