#ifndef IMGUI_IMGUIINTEGRATION_MENUMODULE_HPP
#define IMGUI_IMGUIINTEGRATION_MENUMODULE_HPP

#include <CocktailEngine/Core/Application/App.hpp>

#include <ImGui/ImGuiIntegration/ImUi/Panel/Panel.hpp>

namespace Ck::ImUi
{
    class PanelManager;

    class IMGUI_IMGUIINTEGRATION_API MenuModule : public Observable
    {
    public:

        MenuModule(const MenuModule& other) = delete;
        MenuModule(MenuModule&& other) noexcept = default;
        MenuModule& operator=(const MenuModule& other) = delete;
        MenuModule& operator=(MenuModule&& other) noexcept = default;

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

#endif // IMGUI_IMGUIINTEGRATION_MENUMODULE_HPP
