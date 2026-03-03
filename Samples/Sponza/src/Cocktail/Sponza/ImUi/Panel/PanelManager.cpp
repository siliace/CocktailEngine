#include <Cocktail/Sponza/ImUi/Panel/PanelManager.hpp>

namespace Ck::ImUi
{
    void PanelManager::Update(const Duration& deltaTime)
    {
        for (UniquePtr<Panel>& panel : mPanels)
            panel->Update(deltaTime);
    }

    void PanelManager::Render()
    {
        for (UniquePtr<Panel>& panel : mPanels)
        {
            if (panel->IsOpen())
                panel->Render();
        }
    }
}
