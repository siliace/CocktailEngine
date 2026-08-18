#include <CocktailEngine/Core/System/Network/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck
{
    class SocketInputStream : public InputStream<>
    {
    public:

        explicit SocketInputStream(TcpClient* tcpClient) :
            mTcpClient(tcpClient),
            mReceiveClosed(false)
        {
            /// Nothing
        }

        SizeType Read(Byte* buffer, SizeType length) override
        {
            if (mReceiveClosed || length == 0)
                return 0;

            SizeType received = mTcpClient->Receive(buffer, length);
            if (received == 0)
                mReceiveClosed = true;

            return received;
        }

        bool HasCursor() const override
        {
            return false;
        }

        Uint64 Seek(Uint64 position) override
        {
            ExceptionUtils::ThrowNotImplemented();
        }

        void Rewind() override
        {
            ExceptionUtils::ThrowNotImplemented();
        }

        Uint64 Tell() const override
        {
            ExceptionUtils::ThrowNotImplemented();
        }

        SizeType GetSize() const override
        {
            return mTcpClient->GetPendingLength();
        }

        bool IsEof() const override
        {
            return mReceiveClosed;
        }

    private:

        TcpClient* mTcpClient;
        bool mReceiveClosed;
    };

    class SocketOutputStream : public OutputStream<>
    {
    public:

        explicit SocketOutputStream(TcpClient* tcpClient) :
            mTcpClient(tcpClient)
        {
            /// Nothing
        }

        SizeType Write(const Byte* data, SizeType length) override
        {
            SizeType result = 0;
            for (SizeType sent = 0; sent < length; sent += result)
                result = mTcpClient->Send(data + sent, length - sent);

            return result;
        }

        void Flush() override
        {
        }

    private:

        TcpClient* mTcpClient;
    };

    TcpClient::TcpClient(const IpAddress& remoteAddress, unsigned short port) :
        Socket(remoteAddress.GetProtocol(), Type::Tcp),
        mRemotePort(0)
    {
        assert(port != 0);
        assert(mHandle != InvalidHandle);

        SocketAddress socketAddress = SocketAddress::Of(remoteAddress, port);

        int error = ::connect(mHandle, socketAddress.AsSockAddr(), socketAddress.GetLength());
        if (error == -1)
            throw SystemError::GetLastError();

        mInputStream = MakeUnique<SocketInputStream>(this);
        mOutputStream = MakeUnique<SocketOutputStream>(this);
    }

    InputStream<>& TcpClient::GetInputStream() const
    {
        return *mInputStream;
    }

    OutputStream<>& TcpClient::GetOutputStream() const
    {
        return *mOutputStream;
    }

    const IpAddress* TcpClient::GetRemoteAddress() const
    {
        return mRemoteAddress.Get();
    }

    unsigned short TcpClient::GetRemotePort() const
    {
        return mRemotePort;
    }

    TcpClient::TcpClient(Handle handle, UniquePtr<IpAddress> remoteAddress, unsigned short remotePort) :
        Socket(handle),
        mRemoteAddress(Move(remoteAddress)),
        mRemotePort(remotePort)
    {
        /// Nothing
    }

    unsigned int TcpClient::Send(const void* data, unsigned int length) const
    {
        assert(data && length);
        assert(mHandle != InvalidHandle);

        long result = ::send(mHandle, static_cast<const char*>(data), static_cast<int>(length), 0);
        if (result == -1)
            throw SystemError::GetLastError();

        return static_cast<unsigned int>(result);
    }

    unsigned int TcpClient::Receive(void* data, unsigned int length) const
    {
        assert(data && length);
        assert(mHandle != InvalidHandle);

        long received = ::recv(mHandle, static_cast<char*>(data), static_cast<int>(length), 0);
        if (received == -1)
            throw SystemError::GetLastError();

        return received;
    }
}
