#ifndef COCKTAILENGINE_FTP_CORE_REQUEST_HPP
#define COCKTAILENGINE_FTP_CORE_REQUEST_HPP

#include <CocktailEngine/Core/IO/Output/Writer/StringWriter.hpp>
#include <CocktailEngine/Core/String.hpp>

namespace Ck::Ftp
{
    static const Utf8StringView MessageTerminator(CK_UTF8_TEXT("\r\n"));

    class GenericRequest;

    class Request
    {
    public:

        enum class Code
        {
            Abor,
            Acct,
            Allo,
            Appe,
            Auth,
            Ccc,
            Cdup,
            Conf,
            Cwd,
            Dele,
            Eprt,
            Epsv,
            Feat,
            Help,
            Lang,
            List,
            Mdtm,
            Mff,
            Mkd,
            Mlst,
            Mode,
            Nlst,
            Noop,
            Opts,
            Pass,
            Pasv,
            Pbsz,
            Port,
            Prot,
            Pwd,
            Quit,
            Rein,
            Rest,
            Retr,
            Rmd,
            Rnfr,
            Rnto,
            Site,
            Size,
            Smnt,
            Stat,
            Stor,
            Stou,
            Stru,
            Syst,
            Type,
            User
        };

        /**
         * \brief Convert a command code to its textual representation
         *
         * \param code The command code
         *
         * \return The four (or fewer) letters command mnemonic
         */
        static Utf8StringView CodeToString(Code code);

        /**
         * \brief Convert a textual command mnemonic to its code
         *
         * \param command The command mnemonic (case-insensitive)
         *
         * \return The matching code, or an empty optional for unknown commands
         */
        static Optional<Code> CodeFromString(const Utf8StringView& command);

        /**
         * \brief Parse a raw command line received from a control connection
         *
         * The line is expected without its trailing terminator. The returned
         * request exposes the parsed \ref GetCode and \ref GetArgument.
         *
         * \param line The raw command line (terminator stripped)
         *
         * \return The parsed request, or an empty optional if the command is unknown
         */
        static Optional<GenericRequest> Parse(const Utf8StringView& line);

        virtual ~Request() = default;

        virtual Code GetCode() const = 0;

        /**
         * \brief Retrieve the argument carried by the request, if any
         *
         * Requests that do not take any parameter return an empty view.
         *
         * \return The request argument
         */
        virtual Utf8StringView GetArgument() const
        {
            return Utf8StringView::Empty;
        }

        virtual void Serialize(Writer<Encoders::Utf8>& writer) const = 0;
    };

    namespace Detail
    {
        template <Request::Code T>
        struct Command
        {
        };

        template <>
        struct Command<Request::Code::Abor>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("ABOR"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Acct>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("ACCT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Appe>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("APPE"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Allo>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("ALLO"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Auth>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("AUTH"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Ccc>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("CCC"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Cdup>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("CDUP"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Conf>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("CONF"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Cwd>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("CWD"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Dele>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("DELE"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Eprt>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("EPRT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Epsv>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("EPSV"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Feat>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("FEAT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Help>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("HELP"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Lang>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("LANG"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::List>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("LIST"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Mdtm>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("MDTM"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Mff>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("MFF"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Mkd>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("MKD"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Mlst>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("MLST"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Mode>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("MODE"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Nlst>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("NLST"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Noop>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("NOOP"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Opts>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("OPTS"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Pass>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("PASS"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Pasv>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("PASV"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Pbsz>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("PBSZ"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Port>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("PORT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Prot>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("PROT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Pwd>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("PWD"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Quit>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("QUIT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Rein>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("REIN"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Rest>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("REST"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Retr>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("RETR"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Rmd>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("RMD"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Rnfr>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("RNFR"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Rnto>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("RNTO"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Site>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("SITE"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Size>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("SIZE"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Smnt>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("SMNT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Stat>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("STAT"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Stor>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("STOR"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Stou>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("STOU"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Stru>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("STRU"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Syst>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("SYST"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::Type>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("TYPE"));
                return value;
            }
        };

        template <>
        struct Command<Request::Code::User>
        {
            static Utf8StringView ToString()
            {
                static Utf8StringView value(CK_UTF8_TEXT("USER"));
                return value;
            }
        };

        template <Request::Code TCode>
        class NoParameterRequest : public Request
        {
        public:

            Code GetCode() const override
            {
                return TCode;
            }

            void Serialize(Writer<Encoders::Utf8>& writer) const override
            {
                writer.Write(Command<TCode>::ToString());
                writer.Write(MessageTerminator);
            }
        };

        template <Request::Code TCode>
        class OneParameterRequest : public Request
        {
        public:

            explicit OneParameterRequest(Utf8String parameter) :
                mParameter(Move(parameter))
            {
                /// Nothing
            }

            Code GetCode() const override
            {
                return TCode;
            }

            Utf8StringView GetArgument() const override
            {
                return mParameter;
            }

            void Serialize(Writer<Encoders::Utf8>& writer) const override
            {
                writer.Write(Command<TCode>::ToString());
                writer.Write(CK_UTF8_TEXT(" "));
                writer.Write(mParameter);
                writer.Write(MessageTerminator);
            }

        private:

            Utf8String mParameter;
        };
    }

    /**
     * \brief Request holding a dynamically parsed command and argument
     *
     * Unlike the strongly-typed request aliases below (used to build requests
     * on the client side), this class is produced by \ref Request::Parse to
     * represent a command received on the server side, whatever its code.
     */
    class GenericRequest : public Request
    {
    public:

        GenericRequest(Code code, Utf8String argument) :
            mCode(code),
            mArgument(Move(argument))
        {
            /// Nothing
        }

        Code GetCode() const override
        {
            return mCode;
        }

        Utf8StringView GetArgument() const override
        {
            return mArgument;
        }

        void Serialize(Writer<Encoders::Utf8>& writer) const override
        {
            writer.Write(CodeToString(mCode));
            if (!mArgument.IsEmpty())
            {
                writer.Write(CK_UTF8_TEXT(" "));
                writer.Write(mArgument);
            }
            writer.Write(MessageTerminator);
        }

    private:

        Code mCode;
        Utf8String mArgument;
    };

    using AbortRequest = Detail::NoParameterRequest<Request::Code::Abor>; /// TODO: ABOR may interrupt an ongoing transfer
    using AccountRequest = Detail::OneParameterRequest<Request::Code::Acct>;
    using AllocateRequest = Detail::OneParameterRequest<Request::Code::Allo>;
    using AppendRequest = Detail::OneParameterRequest<Request::Code::Appe>;
    using AuthenticationRequest = Detail::OneParameterRequest<Request::Code::Auth>;
    using ClearCommandChannelRequest = Detail::NoParameterRequest<Request::Code::Ccc>;
    using ChangeToParentDirectoryRequest = Detail::NoParameterRequest<Request::Code::Cdup>;
    using ConfidentialityProtectionCommandRequest = Detail::OneParameterRequest<Request::Code::Conf>;
    using ChangeDirectoryRequest = Detail::OneParameterRequest<Request::Code::Cwd>;
    using DeleteRequest = Detail::OneParameterRequest<Request::Code::Dele>;
    using ExtendedPortRequest = Detail::OneParameterRequest<Request::Code::Eprt>; /// TODO: structured address and port parameter
    using ExtendedPassiveRequest = Detail::NoParameterRequest<Request::Code::Epsv>; /// TODO: optional protocol-family argument
    using FeatureRequest = Detail::NoParameterRequest<Request::Code::Feat>;
    using HelpRequest = Detail::OneParameterRequest<Request::Code::Help>; /// TODO: optional command argument
    using LanguageRequest = Detail::OneParameterRequest<Request::Code::Lang>;
    using ListRequest = Detail::OneParameterRequest<Request::Code::List>; /// TODO: optional path argument
    using ModificationTimeRequest = Detail::OneParameterRequest<Request::Code::Mdtm>;
    using ModifyFactRequest = Detail::OneParameterRequest<Request::Code::Mff>; /// TODO: multiple facts and pathname
    using MakeDirectoryRequest = Detail::OneParameterRequest<Request::Code::Mkd>;
    using MachineListRequest = Detail::OneParameterRequest<Request::Code::Mlst>; /// TODO: optional pathname
    using TransferModeRequest = Detail::OneParameterRequest<Request::Code::Mode>;
    using NameListRequest = Detail::OneParameterRequest<Request::Code::Nlst>; /// TODO: optional path argument
    using NoOperationRequest = Detail::NoParameterRequest<Request::Code::Noop>;
    using OptionsRequest = Detail::OneParameterRequest<Request::Code::Opts>; /// TODO: command-specific options syntax
    using PasswordRequest = Detail::OneParameterRequest<Request::Code::Pass>;
    using PassiveRequest = Detail::NoParameterRequest<Request::Code::Pasv>;
    using ProtectionBufferSizeRequest = Detail::OneParameterRequest<Request::Code::Pbsz>;
    using PortRequest = Detail::OneParameterRequest<Request::Code::Port>; /// TODO: structured host and port argument
    using DataChannelProtectionRequest = Detail::OneParameterRequest<Request::Code::Prot>;
    using PrintWorkingDirectoryRequest = Detail::NoParameterRequest<Request::Code::Pwd>;
    using QuitRequest = Detail::NoParameterRequest<Request::Code::Quit>;
    using ReinitializeRequest = Detail::NoParameterRequest<Request::Code::Rein>;
    using RestartRequest = Detail::OneParameterRequest<Request::Code::Rest>;
    using RetrieveRequest = Detail::OneParameterRequest<Request::Code::Retr>;
    using RemoveDirectoryRequest = Detail::OneParameterRequest<Request::Code::Rmd>;
    using RenameFromRequest = Detail::OneParameterRequest<Request::Code::Rnfr>;
    using RenameToRequest = Detail::OneParameterRequest<Request::Code::Rnto>;
    using SiteParametersRequest = Detail::OneParameterRequest<Request::Code::Site>; /// TODO: server-specific command syntax
    using FileSizeRequest = Detail::OneParameterRequest<Request::Code::Size>;
    using StructureMountRequest = Detail::OneParameterRequest<Request::Code::Smnt>;
    using StatusRequest = Detail::OneParameterRequest<Request::Code::Stat>; /// TODO: optional pathname
    using StoreRequest = Detail::OneParameterRequest<Request::Code::Stor>;
    using StoreUniqueRequest = Detail::OneParameterRequest<Request::Code::Stou>; /// TODO: optional pathname
    using FileStructureRequest = Detail::OneParameterRequest<Request::Code::Stru>;
    using SystemTypeRequest = Detail::NoParameterRequest<Request::Code::Syst>;
    using RepresentationTypeRequest = Detail::OneParameterRequest<Request::Code::Type>;
    using UserRequest = Detail::OneParameterRequest<Request::Code::User>;
}

#endif // COCKTAILENGINE_FTP_CORE_REQUEST_HPP
