#ifndef COCKTAILENGINE_PANELMANAGER_HPP
#define COCKTAILENGINE_PANELMANAGER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Memory/UniquePtr.hpp>

#include <ImGui/ImGuiIntegration/ImUi/Panel/Panel.hpp>

namespace Ck::ImUi
{
    class IMGUI_IMGUIINTEGRATION_API PanelManager
    {
    public:

        PanelManager() = default;
        PanelManager(const PanelManager& other) = delete;
        PanelManager(PanelManager&& other) noexcept = default;
        PanelManager& operator=(const PanelManager& other) = delete;
        PanelManager& operator=(PanelManager&& other) noexcept = default;

        template <typename T, typename... TArgs>
        T* CreatePanel(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<Panel, T>, "T must derive from IPanel");

            UniquePtr<T> panel = MakeUnique<T>(Forward<TArgs>(args)...);
            T* panelPtr = panel.Get();

            panel->Attach();
            mPanels.Add(Move(panel));

            return panelPtr;
        }

        /**
         * \brief
         * \param deltaTime
         */
        void Update(const Duration& deltaTime);

        /**
         * \brief
         */
        void Render();

    private:

        Array<UniquePtr<Panel>> mPanels;
    };
}

#endif // COCKTAILENGINE_PANELMANAGER_HPP
