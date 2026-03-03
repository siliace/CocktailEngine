#ifndef COCKTAIL_SPONZA_IMGUI_IMGUIWINDOW_HPP
#define COCKTAIL_SPONZA_IMGUI_IMGUIWINDOW_HPP

#include <imgui.h>

#include <Cocktail/Core/System/Window/Window.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

#include <Cocktail/Sponza/ImUi/WindowRenderer.hpp>
#include <Cocktail/Sponza/ImUi/WindowResources.hpp>

namespace Ck::ImUi
{
    class PanelManager;
    class MenuBarManager;

    class AbstractContainer : public Observable
    {
    public:

        AbstractContainer();

        PanelManager* GetPanelManager() const;
        MenuBarManager* GetMenuBarManager() const;

    protected:

        UniquePtr<PanelManager> mPanelManager;
        UniquePtr<MenuBarManager> mMenuBarManager;
    };

    class WindowContainer : public AbstractContainer
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

        static std::shared_ptr<Cursor> GetCursor(int mouseCursor);

        Window* mNativeWindow;
        int mLastMouseCursor;
        ImGuiContext* mImGuiContext;
        UniquePtr<WindowRenderer> mWindowRenderer;
        UniquePtr<WindowResources> mWindowResources;
    };
}

#endif // COCKTAIL_SPONZA_IMGUI_IMGUIWINDOW_HPP
