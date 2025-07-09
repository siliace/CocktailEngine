#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/Unix/SocketPollerImpl.hpp>

namespace Ck
{
	void SocketPoller::Impl::Add(Socket::Handle socketHandle, EventFlags flags)
	{
		pollfd descriptor = { socketHandle, 0, 0 };

		if (flags & EventFlagBits::Read)
			descriptor.events |= POLLRDNORM;

		if (flags & EventFlagBits::Write)
			descriptor.events |= POLLWRNORM;

		mDescriptors.Add(descriptor);
	}

	void SocketPoller::Impl::Remove(Socket::Handle socketHandle)
	{
		mDescriptors.FilterInPlace([&](const pollfd& descriptor) {
			return descriptor.fd == socketHandle;
		});
	}

	void SocketPoller::Impl::Clear()
	{
		mDescriptors.Clear();
	}

	bool SocketPoller::Impl::Has(Socket::Handle socketHandle) const
	{
		return GetSocketDescriptor(socketHandle) != nullptr;
	}

	bool SocketPoller::Impl::Wait(const Duration& duration)
	{
		if (mDescriptors.IsEmpty())
			return false;

		Uint64 timeout = duration.GetCount(TimeUnit::Milliseconds());
		int updated = ::poll(mDescriptors.GetData(), mDescriptors.GetSize(), static_cast<int>(timeout));
		if (updated == -1)
			throw SystemError::GetLastError();

		return updated > 0;
	}

	bool SocketPoller::Impl::IsDisconnected(Socket::Handle socket) const
	{
		return (GetSocketDescriptor(socket)->revents & POLLHUP) > 0;
	}

	bool SocketPoller::Impl::IsReadyToRead(Socket::Handle socket) const
	{
		return (GetSocketDescriptor(socket)->revents & POLLRDNORM) > 0;
	}

	bool SocketPoller::Impl::IsReadyToWrite(Socket::Handle socket) const
	{
		return (GetSocketDescriptor(socket)->revents & POLLWRNORM) > 0;
	}

	const pollfd* SocketPoller::Impl::GetSocketDescriptor(Socket::Handle socket) const
	{
		for (const pollfd& descriptor : mDescriptors)
		{
			if (descriptor.fd == socket)
				return &descriptor;
		}

		return nullptr;
	}
}
