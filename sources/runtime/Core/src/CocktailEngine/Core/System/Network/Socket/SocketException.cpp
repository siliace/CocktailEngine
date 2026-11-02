#include <CocktailEngine/Core/System/Network/Socket/SocketException.hpp>

namespace Ck
{
    String SocketErrorCodeTranslator::TranslateErrorCode(SocketErrorCode errorCode) const
    {
        switch (errorCode)
        {
            case SocketErrorCode::Interrupted: return CK_TEXT("Interrupted");
            case SocketErrorCode::BadFileDescriptor: return CK_TEXT("Bad file descriptor");
            case SocketErrorCode::AccessDenied: return CK_TEXT("Access denied");
            case SocketErrorCode::Fault: return CK_TEXT("Bad address");
            case SocketErrorCode::InvalidArgument: return CK_TEXT("Invalid argument");
            case SocketErrorCode::TooManyOpenFiles: return CK_TEXT("Too many open files");

            case SocketErrorCode::WouldBlock: return CK_TEXT("Operation would block");
            case SocketErrorCode::InProgress: return CK_TEXT("Operation in progress");
            case SocketErrorCode::AlreadyInProgress: return CK_TEXT("Operation already in progress");

            case SocketErrorCode::NotSocket: return CK_TEXT("Not a socket");
            case SocketErrorCode::DestinationAddressRequired: return CK_TEXT("Destination address required");
            case SocketErrorCode::MessageTooLarge: return CK_TEXT("Message too large");

            case SocketErrorCode::ProtocolWrongType: return CK_TEXT("Wrong protocol type");
            case SocketErrorCode::ProtocolOptionNotSupported: return CK_TEXT("Protocol option not supported");
            case SocketErrorCode::ProtocolNotSupported: return CK_TEXT("Protocol not supported");
            case SocketErrorCode::SocketTypeNotSupported: return CK_TEXT("Socket type not supported");
            case SocketErrorCode::OperationNotSupported: return CK_TEXT("Operation not supported");
            case SocketErrorCode::ProtocolFamilyNotSupported: return CK_TEXT("Protocol family not supported");
            case SocketErrorCode::AddressFamilyNotSupported: return CK_TEXT("Address family not supported");

            case SocketErrorCode::AddressInUse: return CK_TEXT("Address already in use");
            case SocketErrorCode::AddressNotAvailable: return CK_TEXT("Address not available");

            case SocketErrorCode::NetworkDown: return CK_TEXT("Network is down");
            case SocketErrorCode::NetworkUnreachable: return CK_TEXT("Network unreachable");
            case SocketErrorCode::NetworkReset: return CK_TEXT("Network reset");
            case SocketErrorCode::ConnectionAborted: return CK_TEXT("Connection aborted");
            case SocketErrorCode::ConnectionReset: return CK_TEXT("Connection reset");
            case SocketErrorCode::NoBufferSpace: return CK_TEXT("No buffer space available");
            case SocketErrorCode::IsConnected: return CK_TEXT("Socket is already connected");
            case SocketErrorCode::NotConnected: return CK_TEXT("Socket is not connected");
            case SocketErrorCode::Shutdown: return CK_TEXT("Socket has been shut down");
            case SocketErrorCode::TooManyReferences: return CK_TEXT("Too many references");
            case SocketErrorCode::TimedOut: return CK_TEXT("Connection timed out");
            case SocketErrorCode::ConnectionRefused: return CK_TEXT("Connection refused");

            case SocketErrorCode::TooManySymbolicLinkLevels: return CK_TEXT("Too many symbolic link levels");
            case SocketErrorCode::NameTooLong: return CK_TEXT("Name too long");

            case SocketErrorCode::HostDown: return CK_TEXT("Host is down");
            case SocketErrorCode::HostUnreachable: return CK_TEXT("Host unreachable");

            case SocketErrorCode::NoMemory: return CK_TEXT("Not enough memory");
            case SocketErrorCode::NotSupported: return CK_TEXT("Not supported");
            case SocketErrorCode::OperationCanceled: return CK_TEXT("Operation canceled");

            default: return CK_TEXT("Unknown socket error");
        }
    }
}
