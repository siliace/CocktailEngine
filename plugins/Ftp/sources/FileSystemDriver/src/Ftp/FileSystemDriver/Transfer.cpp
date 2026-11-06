#include <CocktailEngine/Core/System/Network/TcpClient.hpp>

#include <Ftp/Core/Exception.hpp>

#include <Ftp/FileSystemDriver/Transfer.hpp>

namespace Ck::Ftp::Detail
{
        Utf8String ToFtpPath(const Path& path)
        {
            // FTP always uses '/' whatever the local platform is
            return Utf8String::Convert(path.ToFormat(Path::Format::Generic).ToString());
        }

        bool IsDigit(Utf8Char c)
        {
            return c >= CK_UTF8_CHAR('0') && c <= CK_UTF8_CHAR('9');
        }

        bool ParseUnsigned(const Utf8Char* data, std::size_t length, Uint64& value)
        {
            if (length == 0)
                return false;

            value = 0;
            for (std::size_t i = 0; i < length; ++i)
            {
                if (!IsDigit(data[i]))
                    return false;

                value = value * 10 + static_cast<Uint64>(data[i] - CK_UTF8_CHAR('0'));
            }

            return true;
        }

        // Parse a run of digits of a reply starting at position, advancing it
        bool ParseNumber(const Utf8String& text, std::size_t& position, Uint64& value)
        {
            std::size_t start = position;
            while (position < text.GetLength() && IsDigit(text[position]))
                position++;

            return ParseUnsigned(text.GetData() + start, position - start, value);
        }

        // Days since 1970-01-01 of a proleptic gregorian date
        Int64 DaysFromCivil(Int64 year, unsigned int month, unsigned int day)
        {
            year -= month <= 2 ? 1 : 0;
            const Int64 era = (year >= 0 ? year : year - 399) / 400;
            const unsigned int yearOfEra = static_cast<unsigned int>(year - era * 400);
            const unsigned int dayOfYear = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1;
            const unsigned int dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;

            return era * 146097 + static_cast<Int64>(dayOfEra) - 719468;
        }

        // Parse a "time-val" fact: YYYYMMDDHHMMSS[.sss] expressed in UTC
        bool ParseTimeValue(const Utf8Char* data, std::size_t length, Instant& instant)
        {
            if (length < 14)
                return false;

            Uint64 fields[6];
            static const std::size_t sizes[6] = { 4, 2, 2, 2, 2, 2 };

            std::size_t offset = 0;
            for (unsigned int i = 0; i < 6; ++i)
            {
                if (!ParseUnsigned(data + offset, sizes[i], fields[i]))
                    return false;

                offset += sizes[i];
            }

            Uint64 nanoseconds = 0;
            if (offset < length)
            {
                if (data[offset] != CK_UTF8_CHAR('.'))
                    return false;

                Uint64 scale = 100000000;
                for (std::size_t i = offset + 1; i < length; ++i)
                {
                    if (!IsDigit(data[i]))
                        return false;

                    nanoseconds += static_cast<Uint64>(data[i] - CK_UTF8_CHAR('0')) * scale;
                    scale /= 10;
                }
            }

            if (fields[1] < 1 || fields[1] > 12 || fields[2] < 1 || fields[2] > 31 || fields[3] > 23 || fields[4] > 59 || fields[5] > 60)
                return false;

            Int64 days = DaysFromCivil(static_cast<Int64>(fields[0]), static_cast<unsigned int>(fields[1]), static_cast<unsigned int>(fields[2]));
            if (days < 0)
                return false;

            Uint64 seconds = static_cast<Uint64>(days) * 86400 + fields[3] * 3600 + fields[4] * 60 + fields[5];
            instant = Instant::EpochSeconds(seconds, nanoseconds);

            return true;
        }

        bool KeyIs(const Utf8Char* data, std::size_t length, const Utf8Char* key)
        {
            return Utf8StringView(data, static_cast<unsigned int>(length)).Compare(key, false) == 0;
        }

        /**
         * \brief Parse an MLSx entry line "fact=value;fact=value; name" (RFC 3659)
         */
        bool ParseListingEntry(const Utf8Char* data, std::size_t length, ListingEntry& entry)
        {
            std::size_t space = 0;
            while (space < length && data[space] != CK_UTF8_CHAR(' '))
                space++;

            if (space >= length || space + 1 == length)
                return false;

            entry.Name = Utf8String(data + space + 1, static_cast<unsigned int>(length - space - 1));
            entry.Info = PathInfo{};
            entry.Skip = false;

            std::size_t position = 0;
            while (position < space)
            {
                std::size_t end = position;
                while (end < space && data[end] != CK_UTF8_CHAR(';'))
                    end++;

                std::size_t equal = position;
                while (equal < end && data[equal] != CK_UTF8_CHAR('='))
                    equal++;

                if (equal < end)
                {
                    const Utf8Char* key = data + position;
                    std::size_t keyLength = equal - position;
                    const Utf8Char* value = data + equal + 1;
                    std::size_t valueLength = end - equal - 1;

                    if (KeyIs(key, keyLength, CK_UTF8_TEXT("type")))
                    {
                        if (KeyIs(value, valueLength, CK_UTF8_TEXT("file")))
                            entry.Info.Type = PathType::File;
                        else if (KeyIs(value, valueLength, CK_UTF8_TEXT("dir")))
                            entry.Info.Type = PathType::Directory;
                        else if (KeyIs(value, valueLength, CK_UTF8_TEXT("cdir")) || KeyIs(value, valueLength, CK_UTF8_TEXT("pdir")))
                            entry.Skip = true;
                        else
                            entry.Info.Type = PathType::Other;
                    }
                    else if (KeyIs(key, keyLength, CK_UTF8_TEXT("size")))
                    {
                        ParseUnsigned(value, valueLength, entry.Info.Size);
                    }
                    else if (KeyIs(key, keyLength, CK_UTF8_TEXT("modify")))
                    {
                        ParseTimeValue(value, valueLength, entry.Info.LastChangeTime);
                    }
                    else if (KeyIs(key, keyLength, CK_UTF8_TEXT("create")))
                    {
                        ParseTimeValue(value, valueLength, entry.Info.CreationTime);
                    }
                }

                position = end + 1;
            }

            return true;
        }

        // Port to connect the data connection to, from an EPSV (229) reply
        bool ParseExtendedPassivePort(const Utf8String& message, unsigned short& port)
        {
            Optional<unsigned int> open = message.FindFirst(CK_UTF8_CHAR('('));
            if (open.IsEmpty())
                return false;

            std::size_t position = open.Get() + 1;
            if (position + 3 >= message.GetLength())
                return false;

            Utf8Char delimiter = message[position];
            if (message[position + 1] != delimiter || message[position + 2] != delimiter)
                return false;

            position += 3;

            Uint64 value;
            if (!ParseNumber(message, position, value) || value == 0 || value > 65535)
                return false;

            port = static_cast<unsigned short>(value);
            return true;
        }

        // Port to connect the data connection to, from a PASV (227) reply
        bool ParsePassivePort(const Utf8String& message, unsigned short& port)
        {
            Optional<unsigned int> open = message.FindFirst(CK_UTF8_CHAR('('));
            if (open.IsEmpty())
                return false;

            std::size_t position = open.Get() + 1;
            Uint64 numbers[6];
            for (unsigned int i = 0; i < 6; ++i)
            {
                if (!ParseNumber(message, position, numbers[i]) || numbers[i] > 255)
                    return false;

                if (i < 5)
                {
                    if (position >= message.GetLength() || message[position] != CK_UTF8_CHAR(','))
                        return false;

                    position++;
                }
            }

            Uint64 value = numbers[4] * 256 + numbers[5];
            if (value == 0)
                return false;

            port = static_cast<unsigned short>(value);
            return true;
        }

        // Open a passive mode data connection towards the server, it is always established on the control connection host
        void OpenPassiveConnection(const Client& client, TcpClient& data)
        {
            Optional<const IpAddress&> remote = client.GetRemoteAddress();
            if (remote.IsEmpty())
                throw ConnectionClosedException();

            unsigned short port = 0;

            client.Send(ExtendedPassiveRequest());
            Response response = client.WaitResponse();
            if (response.Code == 229 && ParseExtendedPassivePort(response.Message, port))
            {
                data.Connect(remote.Get(), port);
                return;
            }

            // EPSV not supported, fallback to IPv4 only PASV
            client.Send(PassiveRequest());
            response = client.WaitResponse();
            if (response.Code != 227)
                throw Exception(response.Code);

            if (!ParsePassivePort(response.Message, port))
                throw Exception(response.Code);

            data.Connect(remote.Get(), port);
        }

        // Open the data connection and send the transfer command, the transfer starts when this function returns
        void BeginTransfer(const Client& client, const Request& request, TcpClient& data)
        {
            OpenPassiveConnection(client, data);

            client.Send(request);
            Response response = client.WaitResponse();
            if (!response.IsPreliminary())
            {
                data.Close();
                throw Exception(response.Code);
            }
        }

        // Wait for the final reply of a transfer, the data connection must have been closed beforehand
        void EndTransfer(const Client& client)
        {
            Response response = client.WaitResponse();
            if (!response.IsPositiveCompletion())
                throw Exception(response.Code);
        }

        Array<Byte> ReceiveAll(const TcpClient& data)
        {
            Array<Byte> content;

            Byte buffer[4096];
            for (;;)
            {
                unsigned int received = data.Receive(buffer, sizeof(buffer));
                if (received == 0)
                    break;

                content.Append(buffer, received);
            }

            return content;
        }

        // Split MLSx lines and parse them, the lines are separated by LF or CRLF
        void ParseListingLines(const Utf8Char* data, std::size_t length, bool onlyLeadingSpace, Array<ListingEntry>& entries)
        {
            std::size_t position = 0;
            while (position < length)
            {
                std::size_t end = position;
                while (end < length && data[end] != CK_UTF8_CHAR('\n'))
                    end++;

                std::size_t lineEnd = end;
                if (lineEnd > position && data[lineEnd - 1] == CK_UTF8_CHAR('\r'))
                    lineEnd--;

                std::size_t lineStart = position;
                position = end + 1;

                // In a MLST reply, facts lines are the ones starting with a space
                if (onlyLeadingSpace)
                {
                    if (lineStart == lineEnd || data[lineStart] != CK_UTF8_CHAR(' '))
                        continue;

                    lineStart++;
                }

                ListingEntry entry;
                if (lineStart < lineEnd && ParseListingEntry(data + lineStart, lineEnd - lineStart, entry))
                    entries.Add(Move(entry));
            }
        }

        Array<ListingEntry> ListDirectory(const Client& client, const Path& path)
        {
            Utf8String argument = ToFtpPath(path);
            if (argument.IsEmpty())
                argument = Utf8String(CK_UTF8_TEXT("."));

            TcpClient data;
            BeginTransfer(client, MachineListDirectoryRequest(argument), data);

            Array<Byte> content = ReceiveAll(data);
            data.Close();

            EndTransfer(client);

            Array<ListingEntry> entries;
            ParseListingLines(reinterpret_cast<const Utf8Char*>(content.GetData()), content.GetSize(), false, entries);

            return entries;
        }

        Array<Byte> Download(const Client& client, const Path& path)
        {
            TcpClient data;
            BeginTransfer(client, RetrieveRequest(ToFtpPath(path)), data);

            Array<Byte> content = ReceiveAll(data);
            data.Close();

            EndTransfer(client);

            return content;
        }

        void Upload(const Client& client, const Path& path, const Array<Byte>& content)
        {
            TcpClient data;
            BeginTransfer(client, StoreRequest(ToFtpPath(path)), data);

            if (!content.IsEmpty())
                data.Send(content.GetData(), content.GetSize());

            // Closing the data connection marks the end of the file
            data.Close();

            EndTransfer(client);
        }

        PathInfo GetPathInfoFromFallback(const Client& client, const Path& path)
        {
            PathInfo info;

            client.Send(FileSizeRequest(ToFtpPath(path)));
            Response response = client.WaitResponse();
            if (response.Code == 213)
            {
                info.Type = PathType::File;

                std::size_t position = 4;
                ParseNumber(response.Message, position, info.Size);
                return info;
            }

            // SIZE fails on directories, try to enter it to know whether the path exists
            client.Send(PrintWorkingDirectoryRequest());
            response = client.WaitResponse();
            if (response.Code != 257)
                return info;

            Optional<unsigned int> open = response.Message.FindFirst(CK_UTF8_CHAR('"'));
            Optional<unsigned int> close = open.IsEmpty() ? Optional<unsigned int>::Empty() : response.Message.FindFirst(CK_UTF8_CHAR('"'), open.Get() + 1);
            if (open.IsEmpty() || close.IsEmpty())
                return info;

            Utf8String workingDirectory = Utf8String::FromView(response.Message.SubStringView(open.Get() + 1, close.Get() - open.Get() - 1));

            client.Send(ChangeDirectoryRequest(ToFtpPath(path)));
            response = client.WaitResponse();
            if (response.IsPositiveCompletion())
            {
                info.Type = PathType::Directory;

                client.Send(ChangeDirectoryRequest(workingDirectory));
                client.WaitResponse();
            }

            return info;
        }

        // Retrieve the working directory of the session
        Utf8String GetWorkingDirectory(const Client& client)
        {
            client.Send(PrintWorkingDirectoryRequest());
            Response response = client.WaitResponse();
            if (response.Code != 257)
                throw Exception(response.Code);

            Optional<unsigned int> open = response.Message.FindFirst(CK_UTF8_CHAR('"'));
            Optional<unsigned int> close = open.IsEmpty() ? Optional<unsigned int>::Empty() : response.Message.FindFirst(CK_UTF8_CHAR('"'), open.Get() + 1);
            if (open.IsEmpty() || close.IsEmpty())
                throw Exception(response.Code);

            return Utf8String::FromView(response.Message.SubStringView(open.Get() + 1, close.Get() - open.Get() - 1));
        }
}

