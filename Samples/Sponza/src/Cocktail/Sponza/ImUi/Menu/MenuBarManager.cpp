#include <imgui.h>

#include <Cocktail/Sponza/ImUi/Menu/MenuBarManager.hpp>

namespace Ck::ImUi
{
    MenuBarManager::MenuBarManager(PanelManager* panelManager) :
        mPanelManager(panelManager)
    {
        /// Nothing
    }

    void MenuBarManager::Render()
    {
        if (!ImGui::BeginMainMenuBar())
            return;

        for (UniquePtr<MenuModule>& menu : mMenuModules)
        {
            if (!menu->IsEnabled())
                continue;

            if (ImGui::BeginMenu(menu->GetName()))
            {
                menu->Render();
                ImGui::EndMenu();
            }
        }

        ImGui::EndMainMenuBar();
    }
}