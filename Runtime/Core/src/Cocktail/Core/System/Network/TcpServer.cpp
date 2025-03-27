#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/SocketAddress.hpp>
#include <Cocktail/Core/System/Network/TcpServer.hpp>

namespace Ck
{
	TcpServer::TcpServer(unsigned short port, const IpAddress& host, int backlog) :
		Socket(host.GetProtocol(), Type::Tcp)
	{
		assert(port != 0);
		assert(backlog != 0);
		assert(mHandle != InvalidHandle);

		SocketAddress socketAddress = SocketAddress::Of(host, port);
		int error = ::bind(mHandle, socketAddress.AsSockAddr(), socketAddress.GetLength());
		if (error == -1)
			throw SystemError::GetLastError();

		::listen(mHandle, backlog);
		if (error == -1)
			throw SystemError::GetLastError();
	}

	TcpClient TcpServer::Accept() const
	{
		assert(mHandle != InvalidHandle);

		sockaddr_in6 sockAddr;
		int sockaddrlength = sizeof(sockaddr_in6);
		Handle clientHandle = ::accept(mHandle, reinterpret_cast<sockaddr*>(&sockAddr), (socklen_t*)&sockaddrlength);
		if (clientHandle == InvalidHandle)
			throw SystemError::GetLastError();

		SocketAddress socketAddress(reinterpret_cast<const sockaddr&>(sockAddr), sockaddrlength);
		return TcpClient(clientHandle, socketAddress.ToIpAddress(), socketAddress.GetPort());
	}
}
