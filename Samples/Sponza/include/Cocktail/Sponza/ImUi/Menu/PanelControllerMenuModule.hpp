#ifndef COCKTAILENGINE_PANELCONTROLLERMENUMODULE_HPP
#define COCKTAILENGINE_PANELCONTROLLERMENUMODULE_HPP

#include <Cocktail/Sponza/ImUi/Menu/MenuModule.hpp>

#include "Cocktail/Sponza/ImUi/Panel/PanelManager.hpp"

namespace Ck::ImUi
{
    class Panel;

    class PanelControllerMenuModule : public MenuModule
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
