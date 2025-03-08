#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/SocketAddress.hpp>
#include <Cocktail/Core/System/Network/TcpClient.hpp>

namespace Ck
{
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
	}

	void TcpClient::Send(const void* data, std::size_t length) const
	{
		assert(data && length);
		assert(mHandle != InvalidHandle);

		int result;
		for (std::size_t sent = 0; sent < length; sent += static_cast<std::size_t>(result))
		{
			result = ::send(mHandle, static_cast<const char*>(data) + sent, static_cast<int>(length - sent), 0);
			if (result == -1)
				throw SystemError::GetLastError();
		}
	}

	std::size_t TcpClient::Receive(void* data, std::size_t length) const
	{
		assert(data && length);
		assert(mHandle != InvalidHandle);

		int received = ::recv(mHandle, static_cast<char*>(data), static_cast<int>(length), 0);
		if (received == -1)
			throw SystemError::GetLastError();

		return received;
	}

	const IpAddress* TcpClient::GetRemoteAddress() const
	{
		return mRemoteAddress.get();
	}

	unsigned short TcpClient::GetRemotePort() const
	{
		return mRemotePort;
	}

	TcpClient::TcpClient(Handle handle, std::unique_ptr<IpAddress> remoteAddress, unsigned short remotePort):
		Socket(handle),
		mRemoteAddress(std::move(remoteAddress)),
		mRemotePort(remotePort)
	{
		/// Nothing
	}
}
