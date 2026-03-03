#ifndef COCKTAIL_SPONZA_IMGUI_MENUMODULE_HPP
#define COCKTAIL_SPONZA_IMGUI_MENUMODULE_HPP

#include <Cocktail/Core/Application/App.hpp>

#include <Cocktail/Sponza/ImUi/Panel/Panel.hpp>

namespace Ck::ImUi
{
    class PanelManager;

    class MenuModule : public Observable
    {
    public:

        virtual void Attach();

        virtual void Render() = 0;

        bool IsEnabled() const;
        void SetEnabled(bool enabled);

        PanelManager* GetPanelManager() const;

        void SetPanelManager(PanelManager* panelManager);

        virtual const AnsiChar* GetName() const = 0;

    protected:

        MenuModule();

    private:

        bool mEnabled;
        PanelManager* mPanelManager;
    };
}

#endif // COCKTAIL_SPONZA_IMGUI_MENUMODULE_HPP
