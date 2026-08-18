#ifndef IMGUI_IMGUIINTEGRATION_IMGUIWINDOW_HPP
#define IMGUI_IMGUIINTEGRATION_IMGUIWINDOW_HPP

#include <imgui.h>

#include <CocktailEngine/Core/System/Window/Window.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

#include <ImGui/ImGuiIntegration/ImUi/WindowRenderer.hpp>
#include <ImGui/ImGuiIntegration/ImUi/WindowResources.hpp>

namespace Ck::ImUi
{
    class PanelManager;
    class MenuBarManager;

    class IMGUI_IMGUIINTEGRATION_API AbstractContainer : public Observable
    {
    public:

        AbstractContainer();

        PanelManager* GetPanelManager() const;
        MenuBarManager* GetMenuBarManager() const;

    protected:

        UniquePtr<PanelManager> mPanelManager;
        UniquePtr<MenuBarManager> mMenuBarManager;
    };

    class IMGUI_IMGUIINTEGRATION_API WindowContainer : public AbstractContainer
    {
    public:

        /**
         * \brief
         *
         * \param window
         * \param renderDevice
         */
        WindowContainer(Window* window, Renderer::RenderDevice* renderDevice);

        /**
         * \brief
         *
         * \param renderContext
         * \param framebuffer
         * \param deltaTime
         */
        void Render(Renderer::RenderContext* renderContext, Renderer::Framebuffer* framebuffer, Duration deltaTime);

    private:

        static Cursor* GetCursor(int mouseCursor);

        Window* mNativeWindow;
        int mLastMouseCursor;
        ImGuiContext* mImGuiContext;
        UniquePtr<WindowRenderer> mWindowRenderer;
        UniquePtr<WindowResources> mWindowResources;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_IMGUIWINDOW_HPP
