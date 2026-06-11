#include <Cocktail/Core/Signal/Connection.hpp>

namespace Ck
{
	Connection::~Connection()
	{
		Disconnect();
	}

	void Connection::Disconnect()
	{
		if (const SharedPtr<Detail::SlotState> slot = mState.ToStrong())
			slot->Disconnect();

		mState.Reset(nullptr);
	}

	void Connection::Enable() const
	{
		if (const SharedPtr<Detail::SlotState> slot = mState.ToStrong())
			slot->Enable();
	}

	void Connection::Disable() const
	{
		if (const SharedPtr<Detail::SlotState> slot = mState.ToStrong())
			slot->Disable();
	}

	bool Connection::IsConnected() const
	{
		const SharedPtr<Detail::SlotState> slot = mState.ToStrong();
		return slot && slot->IsConnected();
	}

	bool Connection::IsDisabled() const
	{
		const SharedPtr<Detail::SlotState> slot = mState.ToStrong();
		return slot && slot->IsDisabled();
	}

	Connection::Connection(WeakPtr<Detail::SlotState> state) :
		mState(std::move(state))
	{
		/// Nothing
	}
}
