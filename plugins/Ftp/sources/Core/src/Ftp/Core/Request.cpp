#include <Ftp/Core/Request.hpp>

namespace Ck::Ftp
{
    Utf8StringView Request::CodeToString(Code code)
    {
        switch (code)
        {
        case Code::Abor: return Detail::Command<Code::Abor>::ToString();
        case Code::Acct: return Detail::Command<Code::Acct>::ToString();
        case Code::Allo: return Detail::Command<Code::Allo>::ToString();
        case Code::Appe: return Detail::Command<Code::Appe>::ToString();
        case Code::Auth: return Detail::Command<Code::Auth>::ToString();
        case Code::Ccc:  return Detail::Command<Code::Ccc>::ToString();
        case Code::Cdup: return Detail::Command<Code::Cdup>::ToString();
        case Code::Conf: return Detail::Command<Code::Conf>::ToString();
        case Code::Cwd:  return Detail::Command<Code::Cwd>::ToString();
        case Code::Dele: return Detail::Command<Code::Dele>::ToString();
        case Code::Eprt: return Detail::Command<Code::Eprt>::ToString();
        case Code::Epsv: return Detail::Command<Code::Epsv>::ToString();
        case Code::Feat: return Detail::Command<Code::Feat>::ToString();
        case Code::Help: return Detail::Command<Code::Help>::ToString();
        case Code::Lang: return Detail::Command<Code::Lang>::ToString();
        case Code::List: return Detail::Command<Code::List>::ToString();
        case Code::Mdtm: return Detail::Command<Code::Mdtm>::ToString();
        case Code::Mff:  return Detail::Command<Code::Mff>::ToString();
        case Code::Mkd:  return Detail::Command<Code::Mkd>::ToString();
        case Code::Mlst: return Detail::Command<Code::Mlst>::ToString();
        case Code::Mode: return Detail::Command<Code::Mode>::ToString();
        case Code::Nlst: return Detail::Command<Code::Nlst>::ToString();
        case Code::Noop: return Detail::Command<Code::Noop>::ToString();
        case Code::Opts: return Detail::Command<Code::Opts>::ToString();
        case Code::Pass: return Detail::Command<Code::Pass>::ToString();
        case Code::Pasv: return Detail::Command<Code::Pasv>::ToString();
        case Code::Pbsz: return Detail::Command<Code::Pbsz>::ToString();
        case Code::Port: return Detail::Command<Code::Port>::ToString();
        case Code::Prot: return Detail::Command<Code::Prot>::ToString();
        case Code::Pwd:  return Detail::Command<Code::Pwd>::ToString();
        case Code::Quit: return Detail::Command<Code::Quit>::ToString();
        case Code::Rein: return Detail::Command<Code::Rein>::ToString();
        case Code::Rest: return Detail::Command<Code::Rest>::ToString();
        case Code::Retr: return Detail::Command<Code::Retr>::ToString();
        case Code::Rmd:  return Detail::Command<Code::Rmd>::ToString();
        case Code::Rnfr: return Detail::Command<Code::Rnfr>::ToString();
        case Code::Rnto: return Detail::Command<Code::Rnto>::ToString();
        case Code::Site: return Detail::Command<Code::Site>::ToString();
        case Code::Size: return Detail::Command<Code::Size>::ToString();
        case Code::Smnt: return Detail::Command<Code::Smnt>::ToString();
        case Code::Stat: return Detail::Command<Code::Stat>::ToString();
        case Code::Stor: return Detail::Command<Code::Stor>::ToString();
        case Code::Stou: return Detail::Command<Code::Stou>::ToString();
        case Code::Stru: return Detail::Command<Code::Stru>::ToString();
        case Code::Syst: return Detail::Command<Code::Syst>::ToString();
        case Code::Type: return Detail::Command<Code::Type>::ToString();
        case Code::User: return Detail::Command<Code::User>::ToString();
        }

        return Utf8StringView::Empty;
    }

    Optional<Request::Code> Request::CodeFromString(const Utf8StringView& command)
    {
        // The command set is small enough that a linear scan over the known
        // codes is perfectly acceptable and keeps the lookup table in sync
        // with CodeToString automatically.
        static const Code codes[] = {
            Code::Abor, Code::Acct, Code::Allo, Code::Appe, Code::Auth, Code::Ccc,
            Code::Cdup, Code::Conf, Code::Cwd,  Code::Dele, Code::Eprt, Code::Epsv,
            Code::Feat, Code::Help, Code::Lang, Code::List, Code::Mdtm, Code::Mff,
            Code::Mkd,  Code::Mlst, Code::Mode, Code::Nlst, Code::Noop, Code::Opts,
            Code::Pass, Code::Pasv, Code::Pbsz, Code::Port, Code::Prot, Code::Pwd,
            Code::Quit, Code::Rein, Code::Rest, Code::Retr, Code::Rmd,  Code::Rnfr,
            Code::Rnto, Code::Site, Code::Size, Code::Smnt, Code::Stat, Code::Stor,
            Code::Stou, Code::Stru, Code::Syst, Code::Type, Code::User
        };

        for (Code code : codes)
        {
            // FTP commands are case-insensitive.
            if (command.Compare(CodeToString(code), false) == 0)
                return Optional<Code>::Of(code);
        }

        return Optional<Code>::Empty();
    }

    Optional<GenericRequest> Request::Parse(const Utf8StringView& line)
    {
        // A command line is "<command>[ <argument>]"; the command is separated
        // from its argument by the first space.
        Optional<unsigned int> separator = line.FindFirst(CK_UTF8_CHAR(' '));

        Utf8StringView command = separator.Map([&](unsigned int index) {
            return line.SubStringView(0, index);
        }).GetOr(line);

        Optional<Code> code = CodeFromString(command);
        if (code.IsEmpty())
            return Optional<GenericRequest>::Empty();

        Utf8String argument = separator.Map([&](unsigned int index) {
            return Utf8String::FromView(line.SubStringView(index + 1));
        }).GetOr(Utf8String::Empty);

        return Optional<GenericRequest>::Of(InPlace, code.Get(), Move(argument));
    }
}
