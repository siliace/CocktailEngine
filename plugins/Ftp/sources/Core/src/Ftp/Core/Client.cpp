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
        TcpClientOutputStream socketStream(mControlSocket);
        OutputStreamWriter<Encoders::Utf8> writer(socketStream);
        BufferedWriter<Encoders::Utf8, LinearAllocator<1024>> bufferedWriter(writer, 1024);

        request.Serialize(bufferedWriter);

        bufferedWriter.Flush();
    }

    Response Client::WaitResponse() const
    {
        // Receive returns as soon as some bytes are available, a buffered reader would wait for its whole buffer to be filled
        Byte buffer[1024];
        for (;;)
        {
            // Several replies can be received at once (e.g. 150 then 226), only the first one is consumed
            Optional<unsigned int> length = GetResponseLength(mPending);
            if (!length.IsEmpty())
            {
                Utf8String content = mPending.SubString(0, length.Get());
                mPending = mPending.SubString(length.Get());

                return Response::Parse(content);
            }

            unsigned int received = mControlSocket.Receive(buffer, sizeof(buffer));
            if (received == 0)
                throw ConnectionClosedException();

            mPending.Append(reinterpret_cast<const Utf8Char*>(buffer), received);
        }
    }

    Optional<unsigned int> Client::GetResponseLength(const Utf8String& content)
    {
        if (content.GetLength() < MessageTerminator.GetLength())
            return Optional<unsigned int>::Empty();

        Optional<unsigned int> firstTerminator = content.FindFirst(MessageTerminator);
        if (firstTerminator.IsEmpty() || firstTerminator.Get() < 3)
            return Optional<unsigned int>::Empty();

        unsigned int firstLineEnd = firstTerminator.Get() + MessageTerminator.GetLength();

        // A single line reply is "<code> <text>\r\n", a code followed by nothing is accepted as well
        if (content.GetLength() < 4 || content[3] != CK_UTF8_CHAR('-'))
            return Optional<unsigned int>::Of(firstLineEnd);

        // A multi-line reply ends with a line starting with "<code> "
        const Utf8Char* code = content.GetData();

        unsigned int lineStart = firstLineEnd;
        while (lineStart < content.GetLength())
        {
            Optional<unsigned int> terminator = content.FindFirst(MessageTerminator, lineStart);
            if (terminator.IsEmpty())
                return Optional<unsigned int>::Empty();

            unsigned int lineEnd = terminator.Get() + MessageTerminator.GetLength();

            const Utf8Char* line = content.GetData() + lineStart;
            if (terminator.Get() - lineStart >= 4 && line[0] == code[0] && line[1] == code[1] && line[2] == code[2] && line[3] == CK_UTF8_CHAR(' '))
                return Optional<unsigned int>::Of(lineEnd);

            lineStart = lineEnd;
        }

        return Optional<unsigned int>::Empty();
    }
}
