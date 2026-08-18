#include <CocktailEngine/Core/Signal/Detail/SlotContainer.hpp>
#include <CocktailEngine/Core/Signal/Detail/SlotState.hpp>

namespace Ck::Detail
{
    bool SlotState::Disconnect()
    {
        const bool result = DisconnectSilently();
        if (result)
        {
            if (const SharedPtr<SlotContainer> container = mContainer.ToStrong())
                container->Disconnect(this);
        }

        return result;
    }

    void SlotState::Enable()
    {
        mDisabled = false;
    }

    void SlotState::Disable()
    {
        mDisabled = true;
    }

    bool SlotState::IsConnected() const
    {
        return mConnected;
    }

    bool SlotState::IsDisabled() const
    {
        return mDisabled;
    }

    unsigned SlotState::GetGroupId() const
    {
        return mGroupId;
    }

    SlotState::SlotState(WeakPtr<SlotContainer> container, unsigned groupId) :
        mContainer(Move(container)),
        mGroupId(groupId),
        mConnected(true),
        mDisabled(false)
    {
        /// Nothing
    }

    bool SlotState::DisconnectSilently()
    {
        return mConnected.exchange(false);
    }
}
