#ifndef IMGUI_IMGUIINTEGRATION_SCENEMENUMODULE_HPP
#define IMGUI_IMGUIINTEGRATION_SCENEMENUMODULE_HPP

#include <ImGui/ImGuiIntegration/ImUi/Menu/PanelControllerMenuModule.hpp>

namespace Ck
{
    class Scene;
    
    class IMGUI_IMGUIINTEGRATION_API SceneMenuModule : public ImUi::PanelControllerMenuModule
    {
    public:

        explicit SceneMenuModule(Scene* scene);

        void Attach() override;

        const AnsiChar* GetName() const override;

    private:

        Scene* mScene;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_SCENEMENUMODULE_HPP
