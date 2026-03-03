#include <Cocktail/Sponza/ImUi/Menu/MenuModule.hpp>

namespace Ck::ImUi
{
    void MenuModule::Attach()
    {
    }

    bool MenuModule::IsEnabled() const
    {
        return mEnabled;
    }

    void MenuModule::SetEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    PanelManager* MenuModule::GetPanelManager() const
    {
        return mPanelManager;
    }

    void MenuModule::SetPanelManager(PanelManager* panelManager)
    {
        mPanelManager = panelManager;
    }

    MenuModule::MenuModule() :
        mEnabled(true),
        mPanelManager(nullptr)
    {
        /// Nothing
    }
}
