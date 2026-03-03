#ifndef COCKTAIL_SPONZA_IMUI_MENUBARMANAGER_HPP
#define COCKTAIL_SPONZA_IMUI_MENUBARMANAGER_HPP

#include <Cocktail/Sponza/ImUi/Menu/MenuModule.hpp>
#include <Cocktail/Sponza/ImUi/Panel/PanelManager.hpp>

namespace Ck::ImUi
{

    class MenuBarManager
    {
    public:

        explicit MenuBarManager(PanelManager* panelManager);

        template <typename T, typename... Args>
        T* CreateMenu(Args&&... args)
        {
            static_assert(std::is_base_of_v<MenuModule, T>);

            UniquePtr<T> menuModule = MakeUnique<T>(std::forward<Args>(args)...);
            menuModule->SetPanelManager(mPanelManager);
            menuModule->Attach();

            T* menuModulePtr = menuModule.Get();
            mMenuModules.Add(std::move(menuModule));

            return menuModulePtr;
        }

        void Render();

    private:

        PanelManager* mPanelManager;
        Array<UniquePtr<MenuModule>> mMenuModules;
    };
}

#endif // COCKTAIL_SPONZA_IMUI_MENUBARMANAGER_HPP
