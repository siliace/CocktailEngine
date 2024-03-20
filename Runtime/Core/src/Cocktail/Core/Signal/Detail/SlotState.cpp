#include <Cocktail/Core/Signal/Detail/SlotContainer.hpp>
#include <Cocktail/Core/Signal/Detail/SlotState.hpp>

namespace Ck::Detail
{
	bool SlotState::Disconnect()
	{
		bool result = mConnected.exchange(false);
		if (result)
			mContainer->Disconnect(this);

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

	bool SlotState::IsDisabled()
	{
		return mDisabled;
	}

	unsigned SlotState::GetGroupId() const
	{
		return mGroupId;
	}

	SlotState::SlotState(SlotContainer* container, unsigned groupId) :
		mContainer(container),
		mGroupId(groupId),
		mConnected(true),
		mDisabled(false)
	{
		/// Nothing
	}
}
