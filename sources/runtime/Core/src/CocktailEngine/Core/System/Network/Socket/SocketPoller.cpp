#include <CocktailEngine/Core/System/Network/Socket/SocketOwner.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketPoller.hpp>

#ifdef COCKTAIL_OS_WINDOWS
#include <CocktailEngine/Core/System/Network/Socket/Win32/SocketPollerImpl.hpp>
#else
#include <CocktailEngine/Core/System/Network/Socket/Unix/SocketPollerImpl.hpp>
#endif

namespace Ck
{
    SocketPoller::SocketPoller()
    {
        mImpl = new Impl();
    }

    SocketPoller::SocketPoller(SocketPoller&& other) noexcept :
        mImpl(std::exchange(other.mImpl, nullptr))
    {
        /// Nothing
    }

    SocketPoller::~SocketPoller()
    {
        delete mImpl;
    }

    SocketPoller& SocketPoller::operator=(SocketPoller&& other) noexcept
    {
        if (this == &other)
            return *this;

        delete mImpl;
        mImpl = std::exchange(other.mImpl, nullptr);

        return *this;
    }

    void SocketPoller::Add(const SocketOwner& socketOwner, EventFlags eventFlags) const
    {
        mImpl->Add(socketOwner.mSocket.GetSystemHandle(), eventFlags);
    }

    void SocketPoller::Remove(const SocketOwner& socketOwner) const
    {
        mImpl->Remove(socketOwner.mSocket.GetSystemHandle());
    }

    void SocketPoller::Clear() const
    {
        mImpl->Clear();
    }

    bool SocketPoller::Wait(Duration timeout) const
    {
        return mImpl->Wait(timeout);
    }

    bool SocketPoller::IsDisconnected(const SocketOwner& socketOwner) const
    {
        Socket::Handle handle = socketOwner.mSocket.GetSystemHandle();
        assert(mImpl->Has(handle));

        return mImpl->IsDisconnected(handle);
    }

    bool SocketPoller::IsReadyToRead(const SocketOwner& socketOwner) const
    {
        Socket::Handle handle = socketOwner.mSocket.GetSystemHandle();
        assert(mImpl->Has(handle));

        return mImpl->IsReadyToRead(handle);
    }

    bool SocketPoller::IsReadyToWrite(const SocketOwner& socketOwner) const
    {
        Socket::Handle handle = socketOwner.mSocket.GetSystemHandle();
        assert(mImpl->Has(handle));

        return mImpl->IsReadyToWrite(handle);
    }
}
