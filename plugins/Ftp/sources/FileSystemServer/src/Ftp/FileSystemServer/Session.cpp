#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemException.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketException.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketPoller.hpp>

#include <Ftp/FileSystemServer/Session.hpp>

namespace Ck::Ftp
{
    namespace
    {
        constexpr unsigned int TransferBufferSize = 16 * 1024;

        struct DateTime
        {
            Int64 Year;
            unsigned int Month;
            unsigned int Day;
            unsigned int Hour;
            unsigned int Minute;
            unsigned int Second;
        };

        // Convert a number of seconds since 1970-01-01 to a UTC date
        DateTime ToDateTime(const Instant& instant)
        {
            Uint64 seconds = instant.GetSeconds();
            Int64 days = static_cast<Int64>(seconds / 86400);
            Uint64 remaining = seconds % 86400;

            Int64 z = days + 719468;
            Int64 era = (z >= 0 ? z : z - 146096) / 146097;
            Int64 dayOfEra = z - era * 146097;
            Int64 yearOfEra = (dayOfEra - dayOfEra / 1460 + dayOfEra / 36524 - dayOfEra / 146096) / 365;
            Int64 dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100);
            Int64 mp = (5 * dayOfYear + 2) / 153;

            DateTime result;
            result.Day = static_cast<unsigned int>(dayOfYear - (153 * mp + 2) / 5 + 1);
            result.Month = static_cast<unsigned int>(mp < 10 ? mp + 3 : mp - 9);
            result.Year = yearOfEra + era * 400 + (result.Month <= 2 ? 1 : 0);
            result.Hour = static_cast<unsigned int>(remaining / 3600);
            result.Minute = static_cast<unsigned int>((remaining % 3600) / 60);
            result.Second = static_cast<unsigned int>(remaining % 60);

            return result;
        }

        void AppendNumber(Utf8String& out, Uint64 value, unsigned int width = 0, Utf8Char pad = CK_UTF8_CHAR('0'))
        {
            Utf8Char digits[20];
            unsigned int count = 0;
            do
            {
                digits[count++] = static_cast<Utf8Char>(CK_UTF8_CHAR('0') + value % 10);
                value /= 10;
            } while (value != 0);

            for (unsigned int i = count; i < width; ++i)
                out.Append(pad);

            while (count > 0)
                out.Append(digits[--count]);
        }

        // YYYYMMDDHHMMSS, the format of the time-val of RFC 3659
        void AppendTimeValue(Utf8String& out, const Instant& instant)
        {
            DateTime time = ToDateTime(instant);

            AppendNumber(out, static_cast<Uint64>(time.Year), 4);
            AppendNumber(out, time.Month, 2);
            AppendNumber(out, time.Day, 2);
            AppendNumber(out, time.Hour, 2);
            AppendNumber(out, time.Minute, 2);
            AppendNumber(out, time.Second, 2);
        }

        // The date of a "ls -l" line: "Jan  1 12:00" for recent files, "Jan  1  2020" for the others
        void AppendUnixDate(Utf8String& out, const Instant& instant)
        {
            static const Utf8Char* const months[12] = {
                CK_UTF8_TEXT("Jan"), CK_UTF8_TEXT("Feb"), CK_UTF8_TEXT("Mar"), CK_UTF8_TEXT("Apr"),
                CK_UTF8_TEXT("May"), CK_UTF8_TEXT("Jun"), CK_UTF8_TEXT("Jul"), CK_UTF8_TEXT("Aug"),
                CK_UTF8_TEXT("Sep"), CK_UTF8_TEXT("Oct"), CK_UTF8_TEXT("Nov"), CK_UTF8_TEXT("Dec")
            };

            DateTime time = ToDateTime(instant);
            DateTime now = ToDateTime(Instant::Now());

            out.Append(months[(time.Month - 1) % 12]);
            out.Append(CK_UTF8_CHAR(' '));
            AppendNumber(out, time.Day, 2, CK_UTF8_CHAR(' '));
            out.Append(CK_UTF8_CHAR(' '));

            Uint64 age = Instant::Now().GetSeconds() - instant.GetSeconds();
            if (instant.GetSeconds() <= Instant::Now().GetSeconds() && age < 180ull * 86400ull && now.Year >= time.Year)
            {
                AppendNumber(out, time.Hour, 2);
                out.Append(CK_UTF8_CHAR(':'));
                AppendNumber(out, time.Minute, 2);
            }
            else
            {
                out.Append(CK_UTF8_CHAR(' '));
                AppendNumber(out, static_cast<Uint64>(time.Year), 4, CK_UTF8_CHAR(' '));
            }
        }

        bool EqualsIgnoreCase(const Utf8StringView& value, const Utf8Char* text)
        {
            return value.Compare(text, false) == 0;
        }

        // Compare two strings in a time depending only on their length, to not leak the beginning of a password
        bool ConstantTimeEquals(const Utf8String& lhs, const Utf8StringView& rhs)
        {
            unsigned int length = lhs.GetLength() > rhs.GetLength() ? lhs.GetLength() : rhs.GetLength();

            unsigned int difference = lhs.GetLength() ^ rhs.GetLength();
            for (unsigned int i = 0; i < length; ++i)
            {
                unsigned int left = i < lhs.GetLength() ? static_cast<unsigned int>(lhs.GetData()[i]) : 0;
                unsigned int right = i < rhs.GetLength() ? static_cast<unsigned int>(rhs.GetData()[i]) : 0;
                difference |= left ^ right;
            }

            return difference == 0;
        }

        bool ParseUnsigned(const Utf8Char* data, unsigned int length, Uint64& value)
        {
            if (length == 0 || length > 18)
                return false;

            value = 0;
            for (unsigned int i = 0; i < length; ++i)
            {
                if (data[i] < CK_UTF8_CHAR('0') || data[i] > CK_UTF8_CHAR('9'))
                    return false;

                value = value * 10 + static_cast<Uint64>(data[i] - CK_UTF8_CHAR('0'));
            }

            return true;
        }

        // Remove the leading options of a LIST argument ("-la path" => "path")
        Utf8StringView SkipListOptions(Utf8StringView argument)
        {
            while (!argument.IsEmpty() && argument.GetData()[0] == CK_UTF8_CHAR('-'))
            {
                unsigned int end = 0;
                while (end < argument.GetLength() && argument.GetData()[end] != CK_UTF8_CHAR(' '))
                    end++;

                if (end >= argument.GetLength())
                    return Utf8StringView::Empty;

                argument = argument.SubStringView(end + 1);
            }

            return argument;
        }

        // Parse the numbers of a "a,b,c,d,e,f" argument
        bool ParseNumberList(const Utf8StringView& argument, Uint64* numbers, unsigned int count)
        {
            unsigned int position = 0;
            for (unsigned int i = 0; i < count; ++i)
            {
                unsigned int start = position;
                while (position < argument.GetLength() && argument.GetData()[position] != CK_UTF8_CHAR(','))
                    position++;

                if (!ParseUnsigned(argument.GetData() + start, position - start, numbers[i]) || numbers[i] > 255)
                    return false;

                if (i + 1 < count)
                {
                    if (position >= argument.GetLength())
                        return false;

                    position++;
                }
            }

            return position == argument.GetLength();
        }

        void AppendQuoted(Utf8String& out, const Utf8String& path)
        {
            out.Append(CK_UTF8_CHAR('"'));
            for (unsigned int i = 0; i < path.GetLength(); ++i)
            {
                // A quote of the path is doubled (RFC 959)
                if (path.GetData()[i] == CK_UTF8_CHAR('"'))
                    out.Append(CK_UTF8_CHAR('"'));

                out.Append(path.GetData()[i]);
            }
            out.Append(CK_UTF8_CHAR('"'));
        }

        void AppendFacts(Utf8String& out, const PathInfo& info)
        {
            out.Append(CK_UTF8_TEXT("type="));
            out.Append(info.Type == PathType::Directory ? CK_UTF8_TEXT("dir") : CK_UTF8_TEXT("file"));
            out.Append(CK_UTF8_CHAR(';'));

            if (info.Type == PathType::File)
            {
                out.Append(CK_UTF8_TEXT("size="));
                AppendNumber(out, info.Size);
                out.Append(CK_UTF8_CHAR(';'));
            }

            if (info.LastChangeTime.GetSeconds() != 0)
            {
                out.Append(CK_UTF8_TEXT("modify="));
                AppendTimeValue(out, info.LastChangeTime);
                out.Append(CK_UTF8_CHAR(';'));
            }

            if (info.CreationTime.GetSeconds() != 0)
            {
                out.Append(CK_UTF8_TEXT("create="));
                AppendTimeValue(out, info.CreationTime);
                out.Append(CK_UTF8_CHAR(';'));
            }
        }
    }

    Session::Session(const SessionContext& context, Connection connection) :
        mContext(context),
        mConnection(Move(connection)),
        mFinished(false),
        mAuthenticated(false),
        mHasUser(false),
        mFailedLogins(0),
        mHasRenameSource(false),
        mRenameSourceType(PathType::None),
        mRestartOffset(0),
        mQuit(false),
        mActiveEnabled(false),
        mActivePort(0)
    {
        mActiveAddress[0] = mActiveAddress[1] = mActiveAddress[2] = mActiveAddress[3] = 0;
    }

    void Session::Start()
    {
        mThread = Thread::Create(this, CK_TEXT("FtpSession"));
    }

    void Session::Join()
    {
        if (mThread)
            mThread->Join();
    }

    bool Session::IsFinished() const
    {
        return mFinished.load();
    }

    void Session::Reject(unsigned int code, const Utf8Char* message)
    {
        try
        {
            Reply(code, message);
        }
        catch (const Exception&)
        {
            /// The client is already gone
        }

        mFinished.store(true);
    }

    unsigned int Session::Run()
    {
        SocketPoller poller;
        const TcpClient& socket = mConnection.GetSocket();

        try
        {
            // A client stalling in the middle of a command is disconnected
            socket.SetReceiveTimeout(Duration::Seconds(30));
            poller.Add(socket, SocketPoller::EventFlagBits::Read);

            Reply(220, CK_UTF8_TEXT("Cocktail Engine FTP server ready"));

            Instant deadline = Instant::Now().After(mContext.Options->IdleTimeout);
            while (!mQuit && !ShouldStop())
            {
                // Waiting with a timeout lets the session notice the server is stopping
                if (!poller.Wait(Duration::Milliseconds(200)))
                {
                    if (Instant::Now().IsAfter(deadline))
                    {
                        Reply(421, CK_UTF8_TEXT("Timeout, closing control connection"));
                        break;
                    }

                    continue;
                }

                if (!poller.IsReadyToRead(socket) && !poller.IsDisconnected(socket))
                    continue;

                Optional<GenericRequest> request = mConnection.ReadRequest();
                deadline = Instant::Now().After(mContext.Options->IdleTimeout);

                if (request.IsEmpty())
                {
                    Reply(500, CK_UTF8_TEXT("Unknown command"));
                    continue;
                }

                Handle(request.Get());
            }
        }
        catch (const Exception&)
        {
            /// The connection is closed or broken, nothing more can be done for this client
        }

        try
        {
            poller.Remove(socket);
        }
        catch (const Exception&)
        {
            /// Nothing
        }

        mFinished.store(true);

        return 0;
    }

    void Session::Reply(unsigned int code, const Utf8Char* message)
    {
        Reply(code, Utf8String(message));
    }

    void Session::Reply(unsigned int code, Utf8String message)
    {
        Response response;
        response.Code = static_cast<Uint16>(code);
        response.Message = Move(message);

        mConnection.SendResponse(response);
    }

    bool Session::Handle(const Request& request)
    {
        Request::Code code = request.GetCode();
        Utf8StringView argument = request.GetArgument();

        // Commands usable before being logged in
        switch (code)
        {
            case Request::Code::User: HandleUser(argument); return true;
            case Request::Code::Pass: HandlePass(argument); return true;
            case Request::Code::Quit:
                Reply(221, CK_UTF8_TEXT("Goodbye"));
                mQuit = true;
                return true;
            case Request::Code::Noop: Reply(200, CK_UTF8_TEXT("OK")); return true;
            case Request::Code::Syst: Reply(215, CK_UTF8_TEXT("UNIX Type: L8")); return true;
            case Request::Code::Feat:
                Reply(211, Utf8String(CK_UTF8_TEXT(
                    "Extensions supported:\r\n UTF8\r\n SIZE\r\n MDTM\r\n REST STREAM\r\n EPSV\r\n EPRT\r\n"
                    " MLST type*;size*;modify*;create*;\r\nEnd")));
                return true;
            default:
                break;
        }

        if (!mAuthenticated)
        {
            Reply(530, CK_UTF8_TEXT("Please login with USER and PASS"));
            return true;
        }

        try
        {
            switch (code)
            {
                case Request::Code::Type: HandleType(argument); break;
                case Request::Code::Mode:
                    if (EqualsIgnoreCase(argument, CK_UTF8_TEXT("S")))
                        Reply(200, CK_UTF8_TEXT("Mode set to stream"));
                    else
                        Reply(504, CK_UTF8_TEXT("Only the stream mode is supported"));
                    break;
                case Request::Code::Stru:
                    if (EqualsIgnoreCase(argument, CK_UTF8_TEXT("F")))
                        Reply(200, CK_UTF8_TEXT("Structure set to file"));
                    else
                        Reply(504, CK_UTF8_TEXT("Only the file structure is supported"));
                    break;
                case Request::Code::Opts: HandleOpts(argument); break;
                case Request::Code::Pwd: HandlePwd(); break;
                case Request::Code::Cwd: HandleCwd(argument); break;
                case Request::Code::Cdup: HandleCwd(Utf8StringView(CK_UTF8_TEXT(".."))); break;
                case Request::Code::Mkd: HandleMkd(argument); break;
                case Request::Code::Rmd: HandleRmd(argument); break;
                case Request::Code::Dele: HandleDele(argument); break;
                case Request::Code::Rnfr: HandleRnfr(argument); break;
                case Request::Code::Rnto: HandleRnto(argument); break;
                case Request::Code::Size: HandleSize(argument); break;
                case Request::Code::Mdtm: HandleMdtm(argument); break;
                case Request::Code::Rest: HandleRest(argument); break;
                case Request::Code::Pasv: HandlePasv(); break;
                case Request::Code::Epsv: HandleEpsv(argument); break;
                case Request::Code::Port: HandlePort(argument); break;
                case Request::Code::Eprt: HandleEprt(argument); break;
                case Request::Code::List:
                case Request::Code::Nlst:
                case Request::Code::Mlsd: HandleList(argument, code); break;
                case Request::Code::Mlst: HandleMlst(argument); break;
                case Request::Code::Retr: HandleRetr(argument); break;
                case Request::Code::Stor: HandleStore(argument, false); break;
                case Request::Code::Appe: HandleStore(argument, true); break;
                case Request::Code::Abor: Reply(226, CK_UTF8_TEXT("No transfer in progress")); break;
                case Request::Code::Allo: Reply(202, CK_UTF8_TEXT("No storage allocation necessary")); break;
                case Request::Code::Help: Reply(214, CK_UTF8_TEXT("Cocktail Engine FTP server")); break;
                case Request::Code::Stat: Reply(211, CK_UTF8_TEXT("Cocktail Engine FTP server status OK")); break;
                default: Reply(502, CK_UTF8_TEXT("Command not implemented")); break;
            }
        }
        catch (const FileSystemException& exception)
        {
            switch (exception.GetErrorCode())
            {
                case FileSystemErrorCode::NoSpaceOnDevice:
                case FileSystemErrorCode::FileTooLarge:
                    Reply(452, CK_UTF8_TEXT("Insufficient storage space"));
                    break;
                case FileSystemErrorCode::NoSuchFileOrDirectory:
                    Reply(550, CK_UTF8_TEXT("No such file or directory"));
                    break;
                case FileSystemErrorCode::FileExists:
                    Reply(550, CK_UTF8_TEXT("File exists"));
                    break;
                case FileSystemErrorCode::PermissionDenied:
                case FileSystemErrorCode::OperationNotPermitted:
                case FileSystemErrorCode::ReadOnlyFileSystem:
                    Reply(550, CK_UTF8_TEXT("Permission denied"));
                    break;
                case FileSystemErrorCode::DirectoryNotEmpty:
                    Reply(550, CK_UTF8_TEXT("Directory not empty"));
                    break;
                case FileSystemErrorCode::IsADirectory:
                    Reply(550, CK_UTF8_TEXT("Is a directory"));
                    break;
                case FileSystemErrorCode::NotADirectory:
                    Reply(550, CK_UTF8_TEXT("Not a directory"));
                    break;
                default:
                    Reply(451, CK_UTF8_TEXT("Local error in processing"));
                    break;
            }
        }
        catch (const SocketException&)
        {
            // The control connection is broken
            throw;
        }
        catch (const ConnectionClosedException&)
        {
            throw;
        }
        catch (const Exception&)
        {
            Reply(451, CK_UTF8_TEXT("Local error in processing"));
        }

        return true;
    }

    void Session::HandleUser(const Utf8StringView& argument)
    {
        mAuthenticated = false;
        mUser = Utf8String::FromView(argument);
        mHasUser = !argument.IsEmpty();

        if (!mHasUser)
        {
            Reply(501, CK_UTF8_TEXT("Syntax error in parameters"));
            return;
        }

        // The answer does not depend on the user, to not reveal which ones exist
        Reply(331, CK_UTF8_TEXT("Password required"));
    }

    void Session::HandlePass(const Utf8StringView& argument)
    {
        if (!mHasUser)
        {
            Reply(503, CK_UTF8_TEXT("Login with USER first"));
            return;
        }

        const FileSystemServerOptions& options = *mContext.Options;

        bool granted = false;
        if (options.AllowAnonymous && (EqualsIgnoreCase(mUser, CK_UTF8_TEXT("anonymous")) || EqualsIgnoreCase(mUser, CK_UTF8_TEXT("ftp"))))
            granted = true;

        for (const Account& account : options.Accounts)
        {
            bool sameUser = ConstantTimeEquals(account.User, mUser);
            bool samePassword = ConstantTimeEquals(account.Password, argument);
            if (sameUser && samePassword)
                granted = true;
        }

        mHasUser = false;

        if (granted)
        {
            mAuthenticated = true;
            mFailedLogins = 0;
            Reply(230, CK_UTF8_TEXT("User logged in"));
            return;
        }

        // Slow down the guessing of passwords
        Thread::SleepFor(Duration::Seconds(1));

        mFailedLogins++;
        if (mFailedLogins >= 3)
        {
            Reply(421, CK_UTF8_TEXT("Too many failed login attempts"));
            mQuit = true;
            return;
        }

        Reply(530, CK_UTF8_TEXT("Login incorrect"));
    }

    void Session::HandleType(const Utf8StringView& argument)
    {
        // The content is always transferred as is, the representation type is only accepted for compatibility
        if (EqualsIgnoreCase(argument, CK_UTF8_TEXT("I")) || EqualsIgnoreCase(argument, CK_UTF8_TEXT("A")) ||
            EqualsIgnoreCase(argument, CK_UTF8_TEXT("L 8")) || EqualsIgnoreCase(argument, CK_UTF8_TEXT("A N")))
            Reply(200, CK_UTF8_TEXT("Type set"));
        else
            Reply(504, CK_UTF8_TEXT("Type not supported"));
    }

    void Session::HandleOpts(const Utf8StringView& argument)
    {
        if (EqualsIgnoreCase(argument, CK_UTF8_TEXT("UTF8 ON")))
            Reply(200, CK_UTF8_TEXT("UTF8 is always on"));
        else if (argument.StartsWith(Utf8StringView(CK_UTF8_TEXT("MLST")), false))
            Reply(200, CK_UTF8_TEXT("MLST OPTS type;size;modify;create;"));
        else
            Reply(501, CK_UTF8_TEXT("Option not understood"));
    }

    bool Session::Resolve(const Utf8StringView& argument, Array<String>& segments) const
    {
        String text = String::Convert(argument);

        segments.Clear();

        unsigned int position = 0;
        if (!text.IsEmpty() && text[0] == CK_CHAR('/'))
            position = 1;
        else
            segments = mCurrentDirectory;

        while (position <= text.GetLength())
        {
            unsigned int end = position;
            while (end < text.GetLength() && text[end] != CK_CHAR('/'))
                end++;

            String segment = text.SubString(position, end - position);
            position = end + 1;

            if (segment.IsEmpty() || (segment.GetLength() == 1 && segment[0] == CK_CHAR('.')))
                continue;

            if (segment.GetLength() == 2 && segment[0] == CK_CHAR('.') && segment[1] == CK_CHAR('.'))
            {
                // The root is its own parent
                if (!segments.IsEmpty())
                    segments.PopLast();

                continue;
            }

            // These characters would let the driver leave the root directory
            for (unsigned int i = 0; i < segment.GetLength(); ++i)
            {
                if (segment[i] == CK_CHAR('\\') || segment[i] == CK_CHAR(':') || segment[i] < CK_CHAR(' '))
                    return false;
            }

            segments.Add(Move(segment));
        }

        return true;
    }

    Path Session::ToRealPath(const Array<String>& segments) const
    {
        Path path = mContext.Options->Root;
        for (const String& segment : segments)
            path.Join(segment);

        return path;
    }

    Utf8String Session::ToVirtualPath(const Array<String>& segments) const
    {
        Utf8String result;
        if (segments.IsEmpty())
            result.Append(CK_UTF8_CHAR('/'));

        for (const String& segment : segments)
        {
            result.Append(CK_UTF8_CHAR('/'));
            result.Append(Utf8String::Convert(segment));
        }

        return result;
    }

    bool Session::CheckWritable()
    {
        if (!mContext.Options->ReadOnly)
            return true;

        Reply(550, CK_UTF8_TEXT("The server is read-only"));
        return false;
    }

    void Session::HandlePwd()
    {
        Utf8String message;
        AppendQuoted(message, ToVirtualPath(mCurrentDirectory));
        message.Append(CK_UTF8_TEXT(" is the current directory"));

        Reply(257, Move(message));
    }

    void Session::HandleCwd(const Utf8StringView& argument)
    {
        Array<String> segments;
        if (!Resolve(argument, segments))
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        PathInfo info;
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            info = mContext.Driver->GetPathInfo(ToRealPath(segments));
        }

        if (info.Type != PathType::Directory)
        {
            Reply(550, CK_UTF8_TEXT("Not a directory"));
            return;
        }

        mCurrentDirectory = Move(segments);
        Reply(250, CK_UTF8_TEXT("Directory changed"));
    }

    void Session::HandleMkd(const Utf8StringView& argument)
    {
        if (!CheckWritable())
            return;

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            mContext.Driver->CreateDirectory(ToRealPath(segments));
        }

        Utf8String message;
        AppendQuoted(message, ToVirtualPath(segments));
        message.Append(CK_UTF8_TEXT(" created"));

        Reply(257, Move(message));
    }

    void Session::HandleRmd(const Utf8StringView& argument)
    {
        if (!CheckWritable())
            return;

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        DirectoryRemoveOptions options;
        options.IgnoreMissing = false;
        options.Recursive = false;

        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            mContext.Driver->RemoveDirectory(ToRealPath(segments), options);
        }

        // The working directory must stay valid
        if (segments.GetSize() <= mCurrentDirectory.GetSize())
        {
            bool isPrefix = true;
            for (unsigned int i = 0; i < segments.GetSize(); ++i)
            {
                if (segments[i] != mCurrentDirectory[i])
                {
                    isPrefix = false;
                    break;
                }
            }

            if (isPrefix)
                mCurrentDirectory.Clear();
        }

        Reply(250, CK_UTF8_TEXT("Directory removed"));
    }

    void Session::HandleDele(const Utf8StringView& argument)
    {
        if (!CheckWritable())
            return;

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        FileRemoveOptions options;
        options.IgnoreMissing = false;

        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            mContext.Driver->RemoveFile(ToRealPath(segments), options);
        }

        Reply(250, CK_UTF8_TEXT("File removed"));
    }

    void Session::HandleRnfr(const Utf8StringView& argument)
    {
        mHasRenameSource = false;

        if (!CheckWritable())
            return;

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        Path path = ToRealPath(segments);

        PathInfo info;
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            info = mContext.Driver->GetPathInfo(path);
        }

        if (info.Type != PathType::File && info.Type != PathType::Directory)
        {
            Reply(550, CK_UTF8_TEXT("No such file or directory"));
            return;
        }

        mRenameSource = Move(path);
        mRenameSourceType = info.Type;
        mHasRenameSource = true;

        Reply(350, CK_UTF8_TEXT("Ready for RNTO"));
    }

    void Session::HandleRnto(const Utf8StringView& argument)
    {
        if (!mHasRenameSource)
        {
            Reply(503, CK_UTF8_TEXT("Bad sequence of commands, use RNFR first"));
            return;
        }

        mHasRenameSource = false;

        if (!CheckWritable())
            return;

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            Reply(553, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        Path destination = ToRealPath(segments);

        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            if (mRenameSourceType == PathType::Directory)
            {
                DirectoryMoveOptions options;
                options.Overwrite = false;
                mContext.Driver->MoveDirectory(mRenameSource, destination, options);
            }
            else
            {
                FileMoveOptions options;
                options.Overwrite = false;
                mContext.Driver->MoveFile(mRenameSource, destination, options);
            }
        }

        Reply(250, CK_UTF8_TEXT("Renamed"));
    }

    void Session::HandleSize(const Utf8StringView& argument)
    {
        Array<String> segments;
        if (!Resolve(argument, segments))
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        PathInfo info;
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            info = mContext.Driver->GetPathInfo(ToRealPath(segments));
        }

        if (info.Type != PathType::File)
        {
            Reply(550, CK_UTF8_TEXT("Not a regular file"));
            return;
        }

        Utf8String message;
        AppendNumber(message, info.Size);

        Reply(213, Move(message));
    }

    void Session::HandleMdtm(const Utf8StringView& argument)
    {
        Array<String> segments;
        if (!Resolve(argument, segments))
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        PathInfo info;
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            info = mContext.Driver->GetPathInfo(ToRealPath(segments));
        }

        if (info.Type != PathType::File)
        {
            Reply(550, CK_UTF8_TEXT("Not a regular file"));
            return;
        }

        Utf8String message;
        AppendTimeValue(message, info.LastChangeTime);

        Reply(213, Move(message));
    }

    void Session::HandleRest(const Utf8StringView& argument)
    {
        Uint64 offset;
        if (!ParseUnsigned(argument.GetData(), argument.GetLength(), offset))
        {
            Reply(501, CK_UTF8_TEXT("Invalid restart offset"));
            return;
        }

        mRestartOffset = offset;
        Reply(350, CK_UTF8_TEXT("Restarting at the given offset"));
    }

    bool Session::HasDataEndpoint() const
    {
        return mPassiveListener || mActiveEnabled;
    }

    void Session::ResetDataEndpoint()
    {
        mPassiveListener = UniquePtr<TcpListener>();
        mActiveEnabled = false;
    }

    void Session::HandlePasv()
    {
        ResetDataEndpoint();

        Optional<const IpAddress&> local = mConnection.GetSocket().GetLocalAddress();
        if (local.IsEmpty() || local.Get().GetLength() != 4)
        {
            Reply(522, CK_UTF8_TEXT("PASV is only available on IPv4, use EPSV"));
            return;
        }

        mPassiveListener = MakeUnique<TcpListener>(0, local.Get(), -1);

        Optional<unsigned short> port = mPassiveListener->GetLocalPort();
        if (port.IsEmpty())
        {
            ResetDataEndpoint();
            Reply(425, CK_UTF8_TEXT("Can't open a passive connection"));
            return;
        }

        Utf8String message(CK_UTF8_TEXT("Entering Passive Mode ("));
        for (unsigned int i = 0; i < 4; ++i)
        {
            AppendNumber(message, local.Get().GetByte(i));
            message.Append(CK_UTF8_CHAR(','));
        }
        AppendNumber(message, port.Get() >> 8);
        message.Append(CK_UTF8_CHAR(','));
        AppendNumber(message, port.Get() & 0xFF);
        message.Append(CK_UTF8_CHAR(')'));

        Reply(227, Move(message));
    }

    void Session::HandleEpsv(const Utf8StringView& argument)
    {
        if (EqualsIgnoreCase(argument, CK_UTF8_TEXT("ALL")))
        {
            Reply(200, CK_UTF8_TEXT("EPSV ALL accepted"));
            return;
        }

        ResetDataEndpoint();

        Optional<const IpAddress&> local = mConnection.GetSocket().GetLocalAddress();
        if (local.IsEmpty())
        {
            Reply(425, CK_UTF8_TEXT("Can't open a passive connection"));
            return;
        }

        mPassiveListener = MakeUnique<TcpListener>(0, local.Get(), -1);

        Optional<unsigned short> port = mPassiveListener->GetLocalPort();
        if (port.IsEmpty())
        {
            ResetDataEndpoint();
            Reply(425, CK_UTF8_TEXT("Can't open a passive connection"));
            return;
        }

        Utf8String message(CK_UTF8_TEXT("Entering Extended Passive Mode (|||"));
        AppendNumber(message, port.Get());
        message.Append(CK_UTF8_TEXT("|)"));

        Reply(229, Move(message));
    }

    void Session::HandlePort(const Utf8StringView& argument)
    {
        ResetDataEndpoint();

        Uint64 numbers[6];
        if (!ParseNumberList(argument, numbers, 6))
        {
            Reply(501, CK_UTF8_TEXT("Syntax error in parameters"));
            return;
        }

        Uint64 port = numbers[4] * 256 + numbers[5];

        // Only the host which opened the control connection is a valid target, otherwise the server could be used to attack another one
        Optional<const IpAddress&> remote = mConnection.GetSocket().GetRemoteAddress();
        if (remote.IsEmpty() || remote.Get().GetLength() != 4)
        {
            Reply(501, CK_UTF8_TEXT("Active mode is only available on IPv4"));
            return;
        }

        for (unsigned int i = 0; i < 4; ++i)
        {
            if (remote.Get().GetByte(i) != numbers[i])
            {
                Reply(501, CK_UTF8_TEXT("The address must be the one of the client"));
                return;
            }
        }

        if (port < 1024)
        {
            Reply(501, CK_UTF8_TEXT("Privileged ports are refused"));
            return;
        }

        for (unsigned int i = 0; i < 4; ++i)
            mActiveAddress[i] = static_cast<Uint8>(numbers[i]);

        mActivePort = static_cast<unsigned short>(port);
        mActiveEnabled = true;

        Reply(200, CK_UTF8_TEXT("PORT command successful"));
    }

    void Session::HandleEprt(const Utf8StringView& argument)
    {
        ResetDataEndpoint();

        // |protocol|address|port|, the delimiter is the first character
        if (argument.GetLength() < 2)
        {
            Reply(501, CK_UTF8_TEXT("Syntax error in parameters"));
            return;
        }

        Utf8Char delimiter = argument.GetData()[0];

        unsigned int fieldStart[3];
        unsigned int fieldEnd[3];
        unsigned int position = 1;
        for (unsigned int i = 0; i < 3; ++i)
        {
            fieldStart[i] = position;
            while (position < argument.GetLength() && argument.GetData()[position] != delimiter)
                position++;

            if (position >= argument.GetLength())
            {
                Reply(501, CK_UTF8_TEXT("Syntax error in parameters"));
                return;
            }

            fieldEnd[i] = position;
            position++;
        }

        Uint64 protocol;
        Uint64 port;
        if (!ParseUnsigned(argument.GetData() + fieldStart[0], fieldEnd[0] - fieldStart[0], protocol) ||
            !ParseUnsigned(argument.GetData() + fieldStart[2], fieldEnd[2] - fieldStart[2], port) || port > 65535)
        {
            Reply(501, CK_UTF8_TEXT("Syntax error in parameters"));
            return;
        }

        if (protocol != 1)
        {
            Reply(522, CK_UTF8_TEXT("Network protocol not supported, use (1)"));
            return;
        }

        // Dotted IPv4 address
        Uint64 bytes[4];
        unsigned int cursor = fieldStart[1];
        for (unsigned int i = 0; i < 4; ++i)
        {
            unsigned int start = cursor;
            while (cursor < fieldEnd[1] && argument.GetData()[cursor] != CK_UTF8_CHAR('.'))
                cursor++;

            if (!ParseUnsigned(argument.GetData() + start, cursor - start, bytes[i]) || bytes[i] > 255)
            {
                Reply(501, CK_UTF8_TEXT("Invalid address"));
                return;
            }

            if (i < 3)
            {
                if (cursor >= fieldEnd[1])
                {
                    Reply(501, CK_UTF8_TEXT("Invalid address"));
                    return;
                }

                cursor++;
            }
        }

        Optional<const IpAddress&> remote = mConnection.GetSocket().GetRemoteAddress();
        if (remote.IsEmpty() || remote.Get().GetLength() != 4)
        {
            Reply(501, CK_UTF8_TEXT("Active mode is only available on IPv4"));
            return;
        }

        for (unsigned int i = 0; i < 4; ++i)
        {
            if (remote.Get().GetByte(i) != bytes[i])
            {
                Reply(501, CK_UTF8_TEXT("The address must be the one of the client"));
                return;
            }
        }

        if (port < 1024)
        {
            Reply(501, CK_UTF8_TEXT("Privileged ports are refused"));
            return;
        }

        for (unsigned int i = 0; i < 4; ++i)
            mActiveAddress[i] = static_cast<Uint8>(bytes[i]);

        mActivePort = static_cast<unsigned short>(port);
        mActiveEnabled = true;

        Reply(200, CK_UTF8_TEXT("EPRT command successful"));
    }

    bool Session::ConnectData(TcpClient& data)
    {
        try
        {
            if (mPassiveListener)
            {
                Optional<TcpClient> accepted = mPassiveListener->TryAccept(Duration::Seconds(15), Duration::Milliseconds(10));
                mPassiveListener = UniquePtr<TcpListener>();

                if (accepted.IsEmpty())
                    return false;

                // The data connection has to come from the client, not from someone else who found the port
                Optional<const IpAddress&> expected = mConnection.GetSocket().GetRemoteAddress();
                Optional<const IpAddress&> actual = accepted.Get().GetRemoteAddress();
                if (expected.IsEmpty() || actual.IsEmpty() || expected.Get().ToString() != actual.Get().ToString())
                    return false;

                data = Move(accepted.Get());
                return true;
            }

            if (mActiveEnabled)
            {
                mActiveEnabled = false;
                data.Connect(IpAddressV4(mActiveAddress[0], mActiveAddress[1], mActiveAddress[2], mActiveAddress[3]), mActivePort);
                return true;
            }
        }
        catch (const SocketException&)
        {
            /// Handled below
        }

        return false;
    }

    void Session::SendData(const TcpClient& data, const Utf8String& content)
    {
        if (!content.IsEmpty())
            data.Send(content.GetData(), content.GetLength());
    }

    bool Session::CollectListing(const Utf8StringView& argument, bool allowFile, Array<ListEntry>& entries, bool& isDirectory)
    {
        Array<String> segments;
        if (!Resolve(argument, segments))
        {
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return false;
        }

        Path path = ToRealPath(segments);

        LockGuard<Mutex> lock(*mContext.DriverMutex);

        PathInfo info = mContext.Driver->GetPathInfo(path);
        if (info.Type == PathType::File && allowFile)
        {
            isDirectory = false;

            ListEntry entry;
            entry.Name = segments.IsEmpty() ? Utf8String() : Utf8String::Convert(segments.Last());
            entry.Info = info;
            entries.Add(Move(entry));

            return true;
        }

        if (info.Type != PathType::Directory)
        {
            Reply(550, CK_UTF8_TEXT("No such directory"));
            return false;
        }

        UniquePtr<DirectoryIterator> iterator = mContext.Driver->CreateDirectoryIterator(path);
        if (!iterator)
        {
            Reply(550, CK_UTF8_TEXT("Can't list the directory"));
            return false;
        }

        isDirectory = true;
        for (; !iterator->IsEnd(); iterator->Next())
        {
            ListEntry entry;
            entry.Name = Utf8String::Convert(iterator->GetPath().GetFilename().ToString());
            entry.Info = iterator->GetPathInfo();

            if (entry.Name.IsEmpty())
                continue;

            entries.Add(Move(entry));
        }

        return true;
    }

    void Session::HandleList(const Utf8StringView& argument, Request::Code code)
    {
        mRestartOffset = 0;

        if (!HasDataEndpoint())
        {
            Reply(425, CK_UTF8_TEXT("Use PASV or PORT first"));
            return;
        }

        Utf8StringView path = code == Request::Code::Mlsd ? argument : SkipListOptions(argument);

        Array<ListEntry> entries;
        bool isDirectory = false;
        if (!CollectListing(path, code != Request::Code::Mlsd, entries, isDirectory))
        {
            ResetDataEndpoint();
            return;
        }

        Utf8String content;
        for (const ListEntry& entry : entries)
        {
            switch (code)
            {
                case Request::Code::Nlst:
                    content.Append(entry.Name);
                    break;
                case Request::Code::Mlsd:
                    AppendFacts(content, entry.Info);
                    content.Append(CK_UTF8_CHAR(' '));
                    content.Append(entry.Name);
                    break;
                default:
                {
                    bool directory = entry.Info.Type == PathType::Directory;
                    content.Append(directory ? CK_UTF8_TEXT("drwxr-xr-x") : CK_UTF8_TEXT("-rw-r--r--"));
                    content.Append(CK_UTF8_TEXT(" 1 ftp ftp "));
                    AppendNumber(content, directory ? 0 : entry.Info.Size, 12, CK_UTF8_CHAR(' '));
                    content.Append(CK_UTF8_CHAR(' '));
                    AppendUnixDate(content, entry.Info.LastChangeTime);
                    content.Append(CK_UTF8_CHAR(' '));
                    content.Append(entry.Name);
                    break;
                }
            }

            content.Append(MessageTerminator);
        }

        Reply(150, CK_UTF8_TEXT("Here comes the directory listing"));

        TcpClient data;
        if (!ConnectData(data))
        {
            Reply(425, CK_UTF8_TEXT("Can't open data connection"));
            return;
        }

        try
        {
            SendData(data, content);
            data.Close();
        }
        catch (const SocketException&)
        {
            Reply(426, CK_UTF8_TEXT("Connection closed, transfer aborted"));
            return;
        }

        Reply(226, CK_UTF8_TEXT("Directory send OK"));
    }

    void Session::HandleMlst(const Utf8StringView& argument)
    {
        Array<String> segments;
        if (!Resolve(argument, segments))
        {
            Reply(501, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        PathInfo info;
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            info = mContext.Driver->GetPathInfo(ToRealPath(segments));
        }

        if (info.Type != PathType::File && info.Type != PathType::Directory)
        {
            Reply(550, CK_UTF8_TEXT("No such file or directory"));
            return;
        }

        // The facts line is part of the control connection reply, it starts with a space
        Utf8String message(CK_UTF8_TEXT("Listing\r\n "));
        AppendFacts(message, info);
        message.Append(CK_UTF8_CHAR(' '));
        message.Append(ToVirtualPath(segments));
        message.Append(CK_UTF8_TEXT("\r\nEnd"));

        Reply(250, Move(message));
    }

    void Session::HandleRetr(const Utf8StringView& argument)
    {
        Uint64 offset = mRestartOffset;
        mRestartOffset = 0;

        if (!HasDataEndpoint())
        {
            Reply(425, CK_UTF8_TEXT("Use PASV or PORT first"));
            return;
        }

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            ResetDataEndpoint();
            Reply(550, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        UniquePtr<File> file;
        try
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);

            file = mContext.Driver->OpenFile(ToRealPath(segments), FileOpenFlags(FileOpenFlagBits::Read) | FileOpenFlagBits::Existing);
            if (file && offset > 0)
                file->SetCursor(FileCursorMode::Begin, static_cast<Int64>(offset));
        }
        catch (const FileSystemException&)
        {
            file = UniquePtr<File>();
        }

        if (!file)
        {
            ResetDataEndpoint();
            Reply(550, CK_UTF8_TEXT("Can't open the file"));
            return;
        }

        Reply(150, CK_UTF8_TEXT("Opening data connection"));

        TcpClient data;
        if (!ConnectData(data))
        {
            Reply(425, CK_UTF8_TEXT("Can't open data connection"));
            return;
        }

        bool failed = false;
        try
        {
            Byte buffer[TransferBufferSize];
            while (!ShouldStop())
            {
                unsigned int count;
                {
                    LockGuard<Mutex> lock(*mContext.DriverMutex);
                    count = file->Read(buffer, sizeof(buffer));
                }

                if (count == 0)
                    break;

                data.Send(buffer, count);
            }

            data.Close();
        }
        catch (const SocketException&)
        {
            Reply(426, CK_UTF8_TEXT("Connection closed, transfer aborted"));
            return;
        }
        catch (const Exception&)
        {
            failed = true;
        }

        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            file = UniquePtr<File>();
        }

        if (failed)
            Reply(451, CK_UTF8_TEXT("Local error in reading the file"));
        else
            Reply(226, CK_UTF8_TEXT("Transfer complete"));
    }

    void Session::HandleStore(const Utf8StringView& argument, bool append)
    {
        Uint64 offset = mRestartOffset;
        mRestartOffset = 0;

        if (!CheckWritable())
        {
            ResetDataEndpoint();
            return;
        }

        if (!HasDataEndpoint())
        {
            Reply(425, CK_UTF8_TEXT("Use PASV or PORT first"));
            return;
        }

        Array<String> segments;
        if (!Resolve(argument, segments) || segments.IsEmpty())
        {
            ResetDataEndpoint();
            Reply(553, CK_UTF8_TEXT("Invalid path"));
            return;
        }

        FileOpenFlags flags(FileOpenFlagBits::Write);
        if (append)
            flags |= FileOpenFlagBits::Append;
        else if (offset == 0)
            flags |= FileOpenFlagBits::Truncate;

        UniquePtr<File> file;
        try
        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);

            file = mContext.Driver->OpenFile(ToRealPath(segments), flags);
            if (file && !append && offset > 0)
                file->SetCursor(FileCursorMode::Begin, static_cast<Int64>(offset));
        }
        catch (const FileSystemException&)
        {
            file = UniquePtr<File>();
        }

        if (!file)
        {
            ResetDataEndpoint();
            Reply(550, CK_UTF8_TEXT("Can't open the file"));
            return;
        }

        Reply(150, CK_UTF8_TEXT("Opening data connection"));

        TcpClient data;
        if (!ConnectData(data))
        {
            Reply(425, CK_UTF8_TEXT("Can't open data connection"));
            return;
        }

        bool failed = false;
        bool aborted = false;
        try
        {
            Byte buffer[TransferBufferSize];
            while (!ShouldStop())
            {
                unsigned int count = data.Receive(buffer, sizeof(buffer));
                if (count == 0)
                    break;

                LockGuard<Mutex> lock(*mContext.DriverMutex);

                unsigned int written = 0;
                while (written < count)
                {
                    unsigned int chunk = file->Write(buffer + written, count - written);
                    if (chunk == 0)
                        throw FileSystemException(FileSystemErrorCode::IoError);

                    written += chunk;
                }
            }

            data.Close();
        }
        catch (const SocketException&)
        {
            aborted = true;
        }
        catch (const Exception&)
        {
            failed = true;
        }

        {
            LockGuard<Mutex> lock(*mContext.DriverMutex);
            try
            {
                if (!failed && !aborted)
                    file->Flush();
            }
            catch (const Exception&)
            {
                failed = true;
            }

            file = UniquePtr<File>();
        }

        if (aborted)
            Reply(426, CK_UTF8_TEXT("Connection closed, transfer aborted"));
        else if (failed)
            Reply(451, CK_UTF8_TEXT("Local error in writing the file"));
        else
            Reply(226, CK_UTF8_TEXT("Transfer complete"));
    }
}
