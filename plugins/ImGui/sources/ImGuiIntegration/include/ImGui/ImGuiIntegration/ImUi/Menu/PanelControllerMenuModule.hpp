#ifndef COCKTAILENGINE_PANELCONTROLLERMENUMODULE_HPP
#define COCKTAILENGINE_PANELCONTROLLERMENUMODULE_HPP

#include <ImGui/ImGuiIntegration/ImUi/Menu/MenuModule.hpp>
#include <ImGui/ImGuiIntegration/ImUi/Panel/PanelManager.hpp>

namespace Ck::ImUi
{
    class Panel;

    class IMGUI_IMGUIINTEGRATION_API PanelControllerMenuModule : public MenuModule
    {
    public:

        void Render() override;

    protected:

        template <typename T>
        void CreatePanel()
        {
            CreatePanel([&]() {
                return GetPanelManager()->CreatePanel<T>();
            });
        }

        template <typename Callable>
        void CreatePanel(Callable callable)
        {
            mPanels.Add(App::Invoke(callable));
        }

    private:

        Array<Panel*> mPanels;
    };
}

#endif // COCKTAILENGINE_PANELCONTROLLERMENUMODULE_HPP
