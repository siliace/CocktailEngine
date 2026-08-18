#ifndef IMGUI_IMGUIINTEGRATION_SYSTEMMENUMODULE_HPP
#define IMGUI_IMGUIINTEGRATION_SYSTEMMENUMODULE_HPP

#include <ImGui/ImGuiIntegration/ImUi/Menu/PanelControllerMenuModule.hpp>

namespace Ck
{
    class IMGUI_IMGUIINTEGRATION_API SystemMenuModule : public ImUi::PanelControllerMenuModule
    {
    public:

        void Attach() override;

        const AnsiChar* GetName() const override;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_SYSTEMMENUMODULE_HPP
