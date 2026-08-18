#ifndef COCKTAILENGINE_FTP_CORE_CLIENT_HPP
#define COCKTAILENGINE_FTP_CORE_CLIENT_HPP

#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/Utility/TranslatorCast.hpp>

#include <Ftp/Core/Request.hpp>

namespace Ck::Ftp
{
    class Response
    {
    public:

        bool IsPositiveCompletion() const
        {
            return Code >= 200 && Code < 300;
        }

        bool IsIntermediate() const
        {
            return Code >= 100 && Code < 200;
        }

        bool IsNegativeCompletion() const
        {
            return Code >= 400 && Code < 600;
        }

        Uint16 Code;
        Utf8String message;
    };

    class Client
    {
    public:

        explicit Client(const IpAddress& remote, unsigned short port = 21) :
            mSocket(remote, port)
        {
            mResponseBuffer.Reserve(1024);
        }

        Response Send(const Request& request) const
        {
            request.Serialize(mWriter);

            const Utf8String& text = mWriter.ToString();
            mSocket.Send(text.GetData(), text.GetLength() * sizeof(Utf8Char));

            std::size_t responseLength = 0;

            Utf8Char buffer[1024];
            static const Utf8StringView MessageTerminator(CK_UTF8_TEXT("\r\n"));
            while (!mResponseBuffer.EndsWith(MessageTerminator))
            {
                std::size_t received = mSocket.Receive(buffer, 1024 * sizeof(Utf8Char));
                if (received == 0)
                    return {};

                mResponseBuffer.Append(buffer, received);
            }

            unsigned int firstSpace = mResponseBuffer.FindFirst(CK_UTF8_CHAR(' ')).Get();
            Utf8String firstWord = mResponseBuffer.SubString(0, firstSpace);
            Uint16 code = TranslatorCast<Uint16>(firstWord);

            Response response{ code, mResponseBuffer.SubString(firstSpace) };

            mWriter.Clear();
            mResponseBuffer.Clear();

            return response;
        }

    private:

        TcpClient mSocket;
        mutable Utf8String mResponseBuffer;
        mutable StringWriter<Encoders::Utf8> mWriter;
    };
}

#endif // COCKTAILENGINE_FTP_CORE_CLIENT_HPP
