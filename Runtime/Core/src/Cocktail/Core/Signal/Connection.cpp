#include <Cocktail/Core/Signal/Connection.hpp>

namespace Ck
{
	Connection::~Connection()
	{
		Disconnect();
	}

	void Connection::Disconnect()
	{
		if (const std::shared_ptr<Detail::SlotState> slot = mState.lock())
			slot->Disconnect();

		mState.reset();
	}

	void Connection::Enable() const
	{
		if (const std::shared_ptr<Detail::SlotState> slot = mState.lock())
			slot->Enable();
	}

	void Connection::Disable() const
	{
		if (const std::shared_ptr<Detail::SlotState> slot = mState.lock())
			slot->Disable();
	}

	bool Connection::IsConnected() const
	{
		const std::shared_ptr<Detail::SlotState> slot = mState.lock();
		return slot && slot->IsConnected();
	}

	bool Connection::IsDisabled() const
	{
		const std::shared_ptr<Detail::SlotState> slot = mState.lock();
		return slot && slot->IsDisabled();
	}

	Connection::Connection(std::weak_ptr<Detail::SlotState> state) :
		mState(std::move(state))
	{
		/// Nothing
	}
}
