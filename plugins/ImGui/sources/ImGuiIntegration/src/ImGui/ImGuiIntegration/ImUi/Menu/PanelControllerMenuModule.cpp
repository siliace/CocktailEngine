#include <imgui.h>

#include <ImGui/ImGuiIntegration/ImUi/Menu/PanelControllerMenuModule.hpp>

namespace Ck::ImUi
{
    void PanelControllerMenuModule::Render()
    {
        for (Panel* panel : mPanels)
        {
            bool open = panel->IsOpen();
            if (ImGui::MenuItem(panel->GetName(), nullptr, panel->IsOpen()))
                panel->SetOpen(!open);
        }
    }
}
