#include <Cocktail/Core/Signal/Connection.hpp>

namespace Ck
{
	Connection::~Connection()
	{
		Disconnect();
	}

	void Connection::Disconnect()
	{
		if (const Ref<Detail::SlotState> slot = mState.ToStrong())
			slot->Disconnect();

		mState = nullptr;
	}

	void Connection::Enable() const
	{
		if (const Ref<Detail::SlotState> slot = mState.ToStrong())
			slot->Enable();
	}

	void Connection::Disable() const
	{
		if (const Ref<Detail::SlotState> slot = mState.ToStrong())
			slot->Disable();
	}

	bool Connection::IsConnected() const
	{
		const Ref<Detail::SlotState> slot = mState.ToStrong();
		return slot && slot->IsConnected();
	}

	bool Connection::IsDisabled() const
	{
		const Ref<Detail::SlotState> slot = mState.ToStrong();
		return slot && slot->IsDisabled();
	}

	Connection::Connection(WeakRef<Detail::SlotState> state):
		mState(std::move(state))
	{
		/// Nothing
	}
}
