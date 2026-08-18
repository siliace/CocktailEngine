#ifndef IMGUI_IMGUIINTEGRATION_IMUI_MENUBARMANAGER_HPP
#define IMGUI_IMGUIINTEGRATION_IMUI_MENUBARMANAGER_HPP

#include <ImGui/ImGuiIntegration/ImUi/Menu/MenuModule.hpp>
#include <ImGui/ImGuiIntegration/ImUi/Panel/PanelManager.hpp>

namespace Ck::ImUi
{
    class IMGUI_IMGUIINTEGRATION_API MenuBarManager
    {
    public:

        explicit MenuBarManager(PanelManager* panelManager);

        MenuBarManager(const MenuBarManager& other) = delete;
        MenuBarManager(MenuBarManager&& other) noexcept = default;
        MenuBarManager& operator=(const MenuBarManager& other) = delete;
        MenuBarManager& operator=(MenuBarManager&& other) noexcept = default;

        template <typename T, typename... Args>
        T* CreateMenu(Args&&... args)
        {
            static_assert(std::is_base_of_v<MenuModule, T>);

            UniquePtr<T> menuModule = MakeUnique<T>(Forward<Args>(args)...);
            menuModule->SetPanelManager(mPanelManager);
            menuModule->Attach();

            T* menuModulePtr = menuModule.Get();
            mMenuModules.Add(Move(menuModule));

            return menuModulePtr;
        }

        void Render();

    private:

        PanelManager* mPanelManager;
        Array<UniquePtr<MenuModule>> mMenuModules;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_IMUI_MENUBARMANAGER_HPP
