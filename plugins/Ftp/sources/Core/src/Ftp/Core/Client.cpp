#include <CocktailEngine/Core/IO/Input/Reader/BufferedReader.hpp>
#include <CocktailEngine/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/BufferedWriter.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/OutputStreamWriter.hpp>
#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>

#include <Ftp/Core/Client.hpp>

namespace Ck::Ftp
{
    Client::Client(const IpAddress& remote, unsigned short port) :
        mControlSocket(remote, port)
    {
        /// Nothing
    }

    void Client::Send(const Request& request) const
    {
        OutputStreamWriter<Encoders::Utf8> writer(mControlSocket.GetOutputStream());
        BufferedWriter<Encoders::Utf8, LinearAllocator<1024>> bufferedWriter(writer, 1024);

        request.Serialize(bufferedWriter);

        bufferedWriter.Flush();
    }

    Response Client::WaitResponse() const
    {
        InputStreamReader<Encoders::Utf8> reader(mControlSocket.GetInputStream());
        BufferedReader<Encoders::Utf8, LinearAllocator<1024>> bufferedReader(reader, 1024);

        Utf8Char buffer[1024];
        Utf8String responseContent;
        while (!ResponseIsComplete(responseContent))
        {
            std::size_t received = bufferedReader.Read(buffer, 1024);
            if (bufferedReader.IsEof())
                throw ConnectionClosedException();

            assert(received > 0);
            responseContent.Append(buffer, received);
        }

        return Response::Parse(responseContent);
    }

    bool Client::ResponseIsComplete(const Utf8String& response)
    {
        if (response.GetLength() < 5)
            return false;

        if (response[3] == CK_UTF8_CHAR('-'))
        {
            Utf8StringView code = response.SubString(0, 3);

            Optional<unsigned int> lastTerminator = response.FindLastOf(MessageTerminator, 1);
            if (!lastTerminator)
                return false;

            unsigned int lastLineStart = lastTerminator.Get() + MessageTerminator.GetLength();
            if (lastLineStart + 4 > response.GetLength()) // A valid last line will starts with its code then a space
                return false;

            Utf8StringView lastLine = response.SubStringView(lastLineStart);
            if (!lastLine.StartsWith(code) || lastLine[3] != CK_UTF8_CHAR(' '))
                return false;
        }
        else if (response[3] != CK_UTF8_CHAR(' '))
        {
            return false;
        }

        return response.EndsWith(MessageTerminator);
    }
}
