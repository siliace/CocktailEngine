#include <CocktailEngine/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <CocktailEngine/Core/IO/Input/Reader/LineReader.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/BufferedWriter.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/OutputStreamWriter.hpp>
#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>

#include <Ftp/Core/Server.hpp>

namespace Ck::Ftp
{
    Connection::Connection(TcpClient socket) :
        mControlSocket(Move(socket))
    {
        /// Nothing
    }

    Optional<GenericRequest> Connection::ReadRequest()
    {
        TcpClientInputStream socketStream(mControlSocket);
        InputStreamReader<Encoders::Utf8> reader(socketStream);
        LineReader<Encoders::Utf8, HeapAllocator> lineReader(reader);

        Utf8String line;
        if (!lineReader.ReadLine(line, false))
            throw ConnectionClosedException();

        return Request::Parse(line);
    }

    void Connection::SendResponse(const Response& response)
    {
        TcpClientOutputStream socketStream(mControlSocket);
        OutputStreamWriter<Encoders::Utf8> writer(socketStream);
        BufferedWriter<Encoders::Utf8, LinearAllocator<1024>> bufferedWriter(writer, 1024);

        response.Serialize(bufferedWriter);

        bufferedWriter.Flush();
    }

    Server::Server(unsigned short port, const IpAddress& host) :
        mControlSocket(port, host)
    {
        /// Nothing
    }

    Connection Server::Accept()
    {
        return Connection(mControlSocket.Accept());
    }

    Optional<Connection> Server::TryAccept(const Duration& timeout, const Duration& interval)
    {
        Optional<TcpClient> client = mControlSocket.TryAccept(timeout, interval);
        if (client.IsEmpty())
            return Optional<Connection>::Empty();

        return Optional<Connection>::Of(Connection(Move(client.Get())));
    }
}
