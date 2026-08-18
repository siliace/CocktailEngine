#ifndef COCKTAILENGINE_FTP_CORE_CLIENT_HPP
#define COCKTAILENGINE_FTP_CORE_CLIENT_HPP

#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/Utility/TranslatorCast.hpp>

#include <Ftp/Core/Request.hpp>
#include <Ftp/Core/Response.hpp>

namespace Ck::Ftp
{
    class Client
    {
    public:

        explicit Client(const IpAddress& remote, unsigned short port = 21);

        void Send(const Request& request) const;
        Response WaitResponse() const;

    private:

        static bool ResponseIsComplete(const Utf8String& response);

        TcpClient mControlSocket;
    };
}

#endif // COCKTAILENGINE_FTP_CORE_CLIENT_HPP
