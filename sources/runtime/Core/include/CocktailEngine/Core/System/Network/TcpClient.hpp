#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP

#include <CocktailEngine/Core/IO/Input/Stream/InputStream.hpp>
#include <CocktailEngine/Core/IO/Output/Stream/OutputStream.hpp>
#include <CocktailEngine/Core/System/Network/Socket.hpp>
#include <CocktailEngine/Core/Utility/ByteArray.hpp>

namespace Ck
{
    class COCKTAILENGINE_CORE_API TcpClient : public Socket
    {
    public:

        /**
         * \brief
         */
        TcpClient(const IpAddress& remoteAddress, unsigned short port);

        InputStream<>& GetInputStream() const;
        OutputStream<>& GetOutputStream() const;

        /**
         * \brief
         * \return
         */
        const IpAddress* GetRemoteAddress() const;

        /**
         * \brief
         * \return
         */
        unsigned short GetRemotePort() const;

    private:

        friend class TcpServer;
        friend class SocketInputStream;
        friend class SocketOutputStream;

        /**
         * \brief
         * \param handle
         * \param remoteAddress
         * \param remotePort
         */
        TcpClient(Handle handle, UniquePtr<IpAddress> remoteAddress, unsigned short remotePort);

        /**
         * \brief
         * \param data
         * \param length
         * \return
         */
        unsigned int Send(const void* data, unsigned int length) const;

        /**
         * \brief
         * \param data
         * \param length
         * \return
         */
        unsigned int Receive(void* data, unsigned int length) const;

        UniquePtr<IpAddress> mRemoteAddress;
        unsigned short mRemotePort;
        UniquePtr<InputStream<>> mInputStream;
        UniquePtr<OutputStream<>> mOutputStream;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
