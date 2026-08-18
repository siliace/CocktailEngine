#include <CocktailEngine/Core/Utility/TranslatorCast.hpp>

#include <Ftp/Core/Request.hpp> // For MessageTerminator
#include <Ftp/Core/Response.hpp>

namespace Ck::Ftp
{
    void Response::Serialize(Writer<Encoders::Utf8>& writer) const
    {
        // FTP status codes are always three digits (RFC 959).
        Utf8Char digits[3] = {
            static_cast<Utf8Char>(CK_UTF8_CHAR('0') + (Code / 100) % 10),
            static_cast<Utf8Char>(CK_UTF8_CHAR('0') + (Code / 10) % 10),
            static_cast<Utf8Char>(CK_UTF8_CHAR('0') + Code % 10)
        };
        Utf8StringView code(digits, 3);

        Optional<unsigned int> embeddedTerminator = Message.FindFirst(MessageTerminator);
        if (embeddedTerminator.IsEmpty())
        {
            // Single-line reply: "<code> <message>\r\n".
            writer.Write(code);
            writer.Write(CK_UTF8_TEXT(" "));
            writer.Write(Message);
            writer.Write(MessageTerminator);
            return;
        }

        // Multi-line reply: the first line uses "<code>-", every intermediate
        // line is written verbatim and the last line repeats "<code> ".
        writer.Write(code);
        writer.Write(CK_UTF8_TEXT("-"));

        unsigned int lineStart = 0;
        Optional<unsigned int> nextTerminator = embeddedTerminator;
        while (!nextTerminator.IsEmpty())
        {
            unsigned int lineEnd = nextTerminator.Get();
            writer.Write(Message.SubStringView(lineStart, lineEnd - lineStart));
            writer.Write(MessageTerminator);

            lineStart = lineEnd + MessageTerminator.GetLength();
            nextTerminator = Message.FindFirst(MessageTerminator, lineStart);
        }

        // Closing line, repeating the code followed by the remaining text.
        writer.Write(code);
        writer.Write(CK_UTF8_TEXT(" "));
        if (lineStart < Message.GetLength())
            writer.Write(Message.SubStringView(lineStart));
        writer.Write(MessageTerminator);
    }

    Response Response::Parse(const Utf8String& content)
    {
        Uint16 code = TranslatorCast<Uint16>(content.SubStringView(0, 3));
        return { code, content };
    }
}
