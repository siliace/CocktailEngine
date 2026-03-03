#ifndef COCKTAILENGINE_PANELMANAGER_HPP
#define COCKTAILENGINE_PANELMANAGER_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>

#include <Cocktail/Sponza/ImUi/Panel/Panel.hpp>

namespace Ck::ImUi
{
    class PanelManager
    {
    public:

        template <typename T, typename... TArgs>
        T* CreatePanel(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<Panel, T>, "T must derive from IPanel");

            UniquePtr<T> panel = MakeUnique<T>(std::forward<TArgs>(args)...);
            T* panelPtr = panel.Get();

            panel->Attach();
            mPanels.Add(std::move(panel));

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
