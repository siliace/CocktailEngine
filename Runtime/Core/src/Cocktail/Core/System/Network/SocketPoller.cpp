#include <Cocktail/Core/System/Network/SocketPoller.hpp>

#ifdef COCKTAIL_OS_WINDOWS
#include <Cocktail/Core/System/Network/Win32/SocketPollerImpl.hpp>
#else
#include <Cocktail/Core/System/Network/Unix/SocketPollerImpl.hpp>
#endif

namespace Ck
{
	SocketPoller::SocketPoller()
	{
		mImpl = new Impl();
	}

	SocketPoller::~SocketPoller()
	{
		delete mImpl;
	}

	void SocketPoller::Add(const Socket& socket, EventFlags eventFlags) const
	{
		mImpl->Add(socket.GetSystemHandle(), eventFlags);
	}

	void SocketPoller::Remove(const Socket& socket) const
	{
		mImpl->Remove(socket.GetSystemHandle());
	}

	void SocketPoller::Clear() const
	{
		mImpl->Clear();
	}

	bool SocketPoller::Wait(Duration timeout) const
	{
		return mImpl->Wait(timeout);
	}

	bool SocketPoller::IsDisconnected(const Socket& socket) const
	{
		Uint64 handle = socket.GetSystemHandle();
		assert(mImpl->Has(handle));

		return mImpl->IsDisconnected(handle);
	}

	bool SocketPoller::IsReadyToRead(const Socket& socket) const
	{
		Uint64 handle = socket.GetSystemHandle();
		assert(mImpl->Has(handle));

		return mImpl->IsReadyToRead(handle);
	}

	bool SocketPoller::IsReadyToWrite(const Socket& socket) const
	{
		Uint64 handle = socket.GetSystemHandle();
		assert(mImpl->Has(handle));

		return mImpl->IsReadyToWrite(handle);
	}
}
