#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETERRORCODE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETERRORCODE_HPP

namespace Ck
{
    /**
     * \brief Describes an error reported by a native socket operation.
     *
     * Socket exposes this platform-neutral subset through Result. Individual
     * operating systems can report additional native errors, so callers should
     * handle errors appropriate to the operation instead of assuming every enum
     * value is produced on every platform.
     */
    enum class SocketErrorCode
    {
        /** \brief The operation was interrupted. */
        Interrupted,
        /** \brief The supplied native handle was invalid. */
        BadFileDescriptor,
        /** \brief The operation was denied by the operating system. */
        AccessDenied,
        /** \brief A supplied address or buffer was invalid. */
        Fault,
        /** \brief An argument was invalid for the requested operation. */
        InvalidArgument,
        /** \brief The process has reached its open-file limit. */
        TooManyOpenFiles,

        /** \brief A non-blocking operation would have waited. */
        WouldBlock,
        /** \brief A non-blocking operation is in progress. */
        InProgress,
        /** \brief The requested operation is already in progress. */
        AlreadyInProgress,

        /** \brief The native handle does not identify a socket. */
        NotSocket,
        /** \brief A destination address is required for the operation. */
        DestinationAddressRequired,
        /** \brief The supplied message exceeds the transport limit. */
        MessageTooLarge,

        /** \brief The protocol does not support the requested socket type. */
        ProtocolWrongType,
        /** \brief A requested protocol option is unsupported. */
        ProtocolOptionNotSupported,
        /** \brief The requested protocol is unsupported. */
        ProtocolNotSupported,
        /** \brief The requested socket type is unsupported. */
        SocketTypeNotSupported,
        /** \brief The requested operation is unsupported for this socket. */
        OperationNotSupported,
        /** \brief The requested protocol family is unsupported. */
        ProtocolFamilyNotSupported,
        /** \brief The requested address family is unsupported. */
        AddressFamilyNotSupported,

        /** \brief The local address and port are already in use. */
        AddressInUse,
        /** \brief The requested local address is unavailable. */
        AddressNotAvailable,

        /** \brief The network interface is unavailable. */
        NetworkDown,
        /** \brief No route to the requested network exists. */
        NetworkUnreachable,
        /** \brief The network reset the connection. */
        NetworkReset,
        /** \brief The connection was aborted locally or by the network. */
        ConnectionAborted,
        /** \brief The peer reset the connection. */
        ConnectionReset,
        /** \brief The system has no buffer space for the operation. */
        NoBufferSpace,
        /** \brief The socket is already connected. */
        IsConnected,
        /** \brief The socket has no active connection. */
        NotConnected,
        /** \brief The socket has been shut down. */
        Shutdown,
        /** \brief The system has too many references to the resource. */
        TooManyReferences,
        /** \brief The operation exceeded its configured time limit. */
        TimedOut,
        /** \brief The remote peer refused the connection. */
        ConnectionRefused,

        /** \brief Too many symbolic links were followed while resolving a path. */
        TooManySymbolicLinkLevels,
        /** \brief A name exceeds the platform length limit. */
        NameTooLong,

        /** \brief The remote host is down. */
        HostDown,
        /** \brief The remote host cannot be reached. */
        HostUnreachable,

        /** \brief The system could not allocate required memory. */
        NoMemory,
        /** \brief The requested feature is not supported. */
        NotSupported,
        /** \brief The operation was canceled. */
        OperationCanceled
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETERRORCODE_HPP
