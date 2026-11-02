#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETOWNER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETOWNER_HPP

#include <CocktailEngine/Core/System/Network/Socket/Socket.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketException.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \brief Owns the native socket shared by high-level network wrappers.
     *
     * TcpClient, TcpListener, and UdpSocket derive from this class to share
     * exclusive native-socket ownership, local-endpoint queries, and the common
     * socket-option error handling. A SocketOwner cannot be copied; moving it
     * transfers ownership and invalidates the source owner.
     *
     * SocketPoller borrows the owned native handle rather than extending this
     * object's lifetime. Remove the owner from every poller, or clear those
     * pollers, before closing, destroying, moving, or move-assigning it.
     * SocketOwner is not safe for concurrent operations without external
     * synchronization.
     */
    class COCKTAILENGINE_CORE_API SocketOwner
    {
    public:

        /**
         * \brief Constructs an owner with no native socket.
         */
        SocketOwner() = default;

        /**
         * \brief Prevents copying the owned native socket.
         */
        SocketOwner(const SocketOwner& other) = delete;

        /**
         * \brief Transfers socket ownership from another owner.
         *
         * The source owner is left without a native socket. Remove the source
         * from every SocketPoller before moving it.
         *
         * \param other Owner to move from.
         */
        SocketOwner(SocketOwner&& other) noexcept = default;

        /**
         * \brief Prevents copying the owned native socket.
         */
        SocketOwner& operator=(const SocketOwner& other) = delete;

        /**
         * \brief Replaces this owner's socket by moving it from another owner.
         *
         * Any socket currently owned by this instance is released before the
         * transfer. The source owner is left without a native socket. Remove
         * both owners from every SocketPoller before this operation.
         *
         * \param other Owner to move from.
         *
         * \return This owner.
         */
        SocketOwner& operator=(SocketOwner&& other) noexcept = default;

        /**
         * \brief Closes the owned socket, if any.
         */
        virtual ~SocketOwner();

        /**
         * \brief Releases the owned socket and clears the cached local endpoint.
         *
         * Calling Close on an owner without a socket has no observable effect.
         * Remove the owner from every SocketPoller, or clear those pollers,
         * before calling this method.
         */
        virtual void Close();

        /**
         * \brief Gets the IP address currently assigned to the local endpoint.
         *
         * The returned reference is borrowed from an internal cache. It remains
         * valid until the next successful local-endpoint query, Close, a move
         * operation involving this owner, or destruction of this owner.
         *
         * \return An Optional containing the local numeric IP address, or an
         *         empty Optional when this owner has no socket.
         *
         * \throws SocketException If querying the native socket fails.
         */
        Optional<const IpAddress&> GetLocalAddress() const;

        /**
         * \brief Gets the port currently assigned to the local endpoint.
         *
         * \return An Optional containing the local port in host byte order, or
         *         an empty Optional when this owner has no socket.
         *
         * \throws SocketException If querying the native socket fails.
         */
        Optional<unsigned short> GetLocalPort() const;

    protected:

        friend class SocketPoller;

        /**
         * \brief Takes ownership of an existing native socket.
         *
         * \pre \p socket is valid.
         *
         * \param socket Socket whose ownership is transferred here.
         */
        explicit SocketOwner(Socket socket);

        /**
         * \brief Creates the native socket without binding or connecting it.
         *
         * Wrappers create their socket on demand when binding or connecting, so
         * this step only matters to obtain one earlier. It serves the options
         * the operating system consults while binding or connecting, such as
         * address reuse or IPv6-only mode: those have to be written between the
         * creation and the bind or connect that follows, since setting them
         * afterwards changes the socket without affecting an endpoint already
         * established.
         *
         * No consistency is checked between \p protocol and \p type. A derived
         * class is responsible for passing the transport its own operations
         * expect, and decides whether to expose the step by publishing a
         * Create of its own that supplies that transport.
         *
         * \pre This owner owns no socket.
         *
         * \param protocol IP protocol used to select IPv4 or IPv6.
         * \param type Transport semantics of the socket.
         *
         * \throws SocketException If the operating system cannot create the
         *         socket.
         */
        void Create(IpAddress::Protocol protocol, Socket::Type type);

        /**
         * \brief Gets the local endpoint cached from the native socket.
         *
         * The returned reference is borrowed from this owner and is invalidated
         * by a subsequent successful local-endpoint query, Close, a move
         * operation involving this owner, or destruction of this owner.
         *
         * \return An Optional containing the local endpoint, or an empty
         *         Optional when this owner has no socket.
         *
         * \throws SocketException If querying the native socket fails.
         */
        Optional<const SocketAddress&> GetLocalSocketAddress() const;

        /**
         * \brief Reads a socket option or throws when the native call fails.
         *
         * \pre This owner has a valid socket.
         * \pre \p option is applicable to the socket protocol, state, and
         *      target platform.
         *
         * \tparam T Value type declared by \p option.
         *
         * \param option Option to read.
         *
         * \return The value currently reported by the operating system.
         *
         * \throws SocketException If reading the option fails.
         */
        template <typename T>
        T GetSocketOption(SocketOption<T> option) const
        {
            Result<T, SocketErrorCode> result = mSocket.GetOption(option);
            if (result.IsFailure())
                throw SocketException(result.GetError());

            return result.GetValue();
        }

        /**
         * \brief Writes a socket option or throws when the native call fails.
         *
         * \pre This owner has a valid socket.
         * \pre \p option is applicable to the socket protocol, state, and
         *      target platform.
         *
         * \tparam T Value type declared by \p option.
         *
         * \param option Option to write.
         * \param value Value to assign to \p option.
         *
         * \throws SocketException If writing the option fails.
         */
        template <typename T>
        void SetSocketOption(SocketOption<T> option, T value) const
        {
            Result<void, SocketErrorCode> result = mSocket.SetOption(option, value);
            if (result.IsFailure())
                throw SocketException(result.GetError());
        }

        /**
         * \brief Native socket owned exclusively by this wrapper.
         */
        Socket mSocket;

    private:

        /**
         * \brief Cache that backs borrowed local-address references.
         */
        mutable SocketAddress mLocalAddress;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETOWNER_HPP
