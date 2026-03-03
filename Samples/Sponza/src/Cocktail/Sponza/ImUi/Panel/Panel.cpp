#include <Cocktail/Sponza/ImUi/Panel/Panel.hpp>

namespace Ck::ImUi
{

    void Panel::Attach()
    {
    }

    void Panel::Update(const Duration& deltaTime)
    {
    }

    bool Panel::IsOpen() const
    {
        return mOpen;
    }

    void Panel::SetOpen(bool open)
    {
        mOpen = open;
    }

    Panel::Panel() :
        mOpen(false)
    {
        /// Nothing
    }
}
