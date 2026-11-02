#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKET_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKET_HPP

#include <limits>

#include <CocktailEngine/Core/System/Network/Address/IpAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketErrorCode.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketOption.hpp>
#include <CocktailEngine/Core/Utility/Result.hpp>

namespace Ck
{
    namespace Detail
    {
        /**
         * \brief Converts an option value between its public and native form.
         *
         * Socket::SetOption and Socket::GetOption exchange the type declared by
         * a SocketOption, while the native socket API expects a fixed layout for
         * each option. This trait names that layout and converts between the two
         * forms. The size of NativeType is what Socket reports to the native
         * API, so a specialization has to describe the layout the operating
         * system expects exactly, not merely a type able to hold the value.
         *
         * The primary template passes the value through unchanged, which suits
         * the options whose public type already matches the native one.
         * Supporting a new option value type means specializing this trait.
         *
         * \tparam T Public type of the option value.
         */
        template <typename T>
        struct SocketOptionTraits
        {
            using NativeType = T;

            static NativeType ToNative(T value)
            {
                return value;
            }

            static T FromNative(NativeType value)
            {
                return value;
            }
        };

        /**
         * \brief Carries a boolean option as the integer flag the native API uses.
         */
        template <>
        struct SocketOptionTraits<bool>
        {
            using NativeType = int;

            static NativeType ToNative(bool value)
            {
                return value ? 1 : 0;
            }

            static bool FromNative(NativeType value)
            {
                return value != 0;
            }
        };

        /**
         * \brief Maps SocketLinger onto the linger structure of the native API.
         *
         * The native layout is a pair of fields holding the enable flag and the
         * timeout. Its field widths are platform-defined and are not the same on
         * every system, so SocketLingerNative has to match the definition of the
         * target platform for SO_LINGER to be read and written correctly.
         */
        template <>
        struct SocketOptionTraits<SocketLinger>
        {
#ifdef COCKTAIL_OS_WINDOWS
            struct SocketLingerNative
            {
                unsigned short OnOff;
                unsigned short Linger;
            };
#else
            struct SocketLingerNative
            {
                int OnOff;
                int Linger;
            };
#endif

            using NativeType = SocketLingerNative;

            static NativeType ToNative(const SocketLinger& value)
            {
                NativeType native;
                native.OnOff = value.Enabled ? 1 : 0;
                native.Linger = static_cast<decltype(native.Linger)>(value.Timeout.GetCount(TimeUnit::Seconds()));

                return native;
            }

            static SocketLinger FromNative(const NativeType& value)
            {
                SocketLinger linger;
                linger.Enabled = value.OnOff != 0;
                linger.Timeout = Duration::Seconds(static_cast<unsigned short>(value.Linger));

                return linger;
            }
        };
        /**
         * \brief Maps a Duration onto the timeout layout of the native API.
         *
         * The platforms disagree on the layout and on the resolution of a socket
         * timeout. Unix-like systems take a pair of fields holding whole seconds
         * and microseconds, while Windows takes a single count of milliseconds,
         * so a Duration is normalised to whichever form the target expects.
         * Precision finer than a millisecond should not be relied on: Windows
         * cannot express it, and Linux rounds a timeout up to the next whole
         * millisecond, so reading the option back can report more time than was
         * asked for.
         *
         * Both platforms read a native value of zero as never timing out, which
         * is what Duration::Infinite denotes here. An infinite Duration converts
         * to zero instead of a saturated count, and a native zero is reported
         * back as Duration::Infinite. One consequence is that a zero-length
         * Duration cannot be told apart from the absence of a timeout, the
         * native API having no encoding for waiting not at all.
         *
         * A Duration larger than the native field can hold is clamped to that
         * field rather than wrapping around. On Windows the ceiling is about
         * forty-nine days.
         */
        template <>
        struct SocketOptionTraits<Duration>
        {
#ifdef COCKTAIL_OS_WINDOWS
            using NativeType = unsigned int;
#else
            struct NativeType
            {
                long int Seconds;
                long MicroSeconds;
            };
#endif

            static NativeType ToNative(const Duration& value)
            {
#ifdef COCKTAIL_OS_WINDOWS
                if (value.IsInfinite())
                    return 0;

                Uint64 milliseconds = value.GetCount(TimeUnit::Milliseconds());
                Uint64 ceiling = static_cast<Uint64>(std::numeric_limits<NativeType>::max());

                return static_cast<NativeType>(milliseconds < ceiling ? milliseconds : ceiling);
#else
                using SecondsType = decltype(NativeType::Seconds);
                using MicroSecondsType = decltype(NativeType::MicroSeconds);

                NativeType native = { 0, 0 };
                if (value.IsInfinite())
                    return native;

                Uint64 microseconds = value.GetCount(TimeUnit::Microseconds());
                Uint64 seconds = microseconds / 1000000;
                Uint64 ceiling = static_cast<Uint64>(std::numeric_limits<SecondsType>::max());

                if (seconds >= ceiling)
                {
                    native.Seconds = std::numeric_limits<SecondsType>::max();

                    return native;
                }

                native.Seconds = static_cast<SecondsType>(seconds);
                native.MicroSeconds = static_cast<MicroSecondsType>(microseconds % 1000000);

                return native;
#endif
            }

            static Duration FromNative(const NativeType& value)
            {
#ifdef COCKTAIL_OS_WINDOWS
                if (value == 0)
                    return Duration::Infinite();

                return Duration::Milliseconds(value);
#else
                if (value.Seconds == 0 && value.MicroSeconds == 0)
                    return Duration::Infinite();

                return Duration::Microseconds(static_cast<Uint64>(value.Seconds) * 1000000 + static_cast<Uint64>(value.MicroSeconds));
#endif
            }
        };

        /**
         * \brief Carries an IPv4 address option as the native 32-bit address.
         *
         * The native form is the one an in_addr holds, that is the four octets
         * in network order, first octet at the lowest address. The conversion
         * copies the octets one by one rather than reusing
         * IpAddressV4::ToInteger, whose packed result is in host order and would
         * reverse the address on a little-endian machine. Going through the
         * octets keeps this independent of the byte order of the host, and
         * matches what SocketAddressConverter does when it fills a socket
         * address.
         */
        template <>
        struct SocketOptionTraits<IpAddressV4>
        {
            using NativeType = unsigned int;

            static NativeType ToNative(const IpAddressV4& value)
            {
                NativeType native = 0;
                Uint8* bytes = reinterpret_cast<Uint8*>(&native);

                for (std::size_t i = 0; i < value.GetLength(); i++)
                    bytes[i] = value.GetByte(i);

                return native;
            }

            static IpAddressV4 FromNative(const NativeType& value)
            {
                const Uint8* bytes = reinterpret_cast<const Uint8*>(&value);

                return IpAddressV4(bytes[0], bytes[1], bytes[2], bytes[3]);
            }
        };
    }

    /**
     * \brief Owns a native network socket.
     *
     * A Socket starts invalid and becomes usable after a successful call to
     * Create. It owns its native handle exclusively, closes it when destroyed,
     * and may be moved but not copied. Operations that can fail at the socket
     * API boundary return a Result whose error is a SocketErrorCode.
     *
     * Behaviour that the operating system exposes as a socket option is read and
     * written through GetOption and SetOption, using the constants declared in
     * the SocketOptions namespace. Blocking mode is not one of them and is
     * handled by SetBlocking.
     *
     * The handle returned by GetSystemHandle is borrowed. Callers must not close
     * it or change its blocking mode outside this class. A Socket is not safe for
     * concurrent operations without external synchronization.
     */
    class COCKTAILENGINE_CORE_API Socket final
    {
    public:

#ifdef COCKTAIL_OS_WINDOWS
        /**
         * \brief Native socket-handle type on Windows.
         */
        using Handle = Uint64;
#else
        /**
         * \brief Native socket-handle type on Unix-like platforms.
         */
        using Handle = int;
#endif

        /**
         * \brief Specifies the transport semantics of a socket.
         */
        enum class Type
        {
            Tcp, /**< Reliable byte-stream transport */
            Udp, /**< Connectionless datagram transport */
            Raw, /**< Raw IP socket, availability may require OS privileges */
        };

        /**
         * \brief Sentinel that represents an invalid native handle.
         */
        static constexpr Handle InvalidHandle = static_cast<Handle>(-1);

        /**
         * \brief Constructs an invalid socket that owns no native resource.
         */
        Socket();

        Socket(const Socket& other) = delete;

        /**
         * \brief Transfers native-handle ownership from another socket.
         *
         * After the transfer, the source is invalid and this instance owns the
         * handle that the source previously owned.
         *
         * \param other Socket to move from.
         */
        Socket(Socket&& other) noexcept;

        /**
         * \brief Closes the owned native socket, if any.
         */
        ~Socket();

        Socket& operator=(const Socket& other) = delete;

        /**
         * \brief Replaces this socket by taking ownership from another socket.
         *
         * Any handle currently owned by this instance is released before the
         * transfer. After the transfer, the source is invalid.
         *
         * \param other Socket to move from.
         *
         * \return This socket.
         */
        Socket& operator=(Socket&& other) noexcept;

        /**
         * \brief Creates a native socket for an IP protocol and transport type.
         *
         * \pre This socket is invalid.
         *
         * \param protocol IP protocol used to select IPv4 or IPv6.
         * \param type Transport semantics of the socket.
         *
         * \return A successful Result when this instance owns a valid socket;
         *         otherwise the error returned by the operating system.
         */
        Result<void, SocketErrorCode> Create(IpAddress::Protocol protocol, Type type);

        /**
         * \brief Releases the native socket and makes this instance invalid.
         *
         * Calling Close on an invalid socket has no observable effect.
         */
        void Close();

        /**
         * \brief Accepts one connection on a listening stream socket.
         *
         * \pre This socket is valid and has been put into listening mode.
         *
         * \param socketAddress Receives a copy of the peer address on success.
         *
         * \return A Socket that owns the accepted connection, or the operating
         *         system error. \p socketAddress is unchanged on failure.
         */
        Result<Socket, SocketErrorCode> Accept(SocketAddress& socketAddress) const;

        /**
         * \brief Binds the socket to a local numeric IP address and port.
         *
         * \pre This socket is valid.
         *
         * \param localAddress Numeric local IPv4 or IPv6 address.
         * \param localPort Local port in host byte order. A value of zero asks
         *        the operating system to select an ephemeral port where supported.
         *
         * \return A successful Result, or the operating system error.
         */
        Result<void, SocketErrorCode> Bind(const IpAddress& localAddress, unsigned short localPort) const;

        /**
         * \brief Connects the socket to a remote numeric IP address and port.
         *
         * \pre This socket is valid.
         *
         * \param address Numeric remote IPv4 or IPv6 address.
         * \param port Remote port in host byte order.
         *
         * \return A successful Result, or the operating system error. For a
         *         non-blocking socket, InProgress can indicate that the
         *         connection is still being established.
         */
        Result<void, SocketErrorCode> Connect(const IpAddress& address, unsigned short port) const;

        /**
         * \brief Marks a stream socket as listening for incoming connections.
         *
         * \pre This socket is valid and has been bound when the target platform
         *      requires an explicit bind.
         *
         * \param backlog Maximum number of queued connection requests. Platform
         *        rules apply to negative values.
         *
         * \return A successful Result, or the operating system error.
         */
        Result<void, SocketErrorCode> Listen(int backlog) const;

        /**
         * \brief Receives up to a requested number of bytes from a connected socket.
         *
         * \pre This socket is valid.
         * \pre \p data is not null and \p length is non-zero.
         * \pre \p length fits in the signed length type accepted by the native API.
         *
         * \param data Buffer to fill.
         * \param length Maximum number of bytes to receive.
         *
         * \return The number of received bytes, which can be less than \p length,
         *         or an operating system error. For a stream socket, zero bytes
         *         on success indicates an orderly remote shutdown.
         */
        Result<unsigned int, SocketErrorCode> Receive(void* data, unsigned int length) const;

        /**
         * \brief Receives one datagram and its sender address.
         *
         * \pre This socket is valid.
         * \pre \p data is not null and \p length is non-zero.
         * \pre \p length fits in the signed length type accepted by the native API.
         *
         * \param data Buffer to fill.
         * \param length Maximum number of bytes to receive.
         * \param socketAddress Receives a copy of the sender address on success.
         *
         * \return The number of received bytes, or an operating system error.
         *         \p socketAddress is unchanged on failure.
         */
        Result<unsigned int, SocketErrorCode> ReceiveFrom(void* data, unsigned int length, SocketAddress& socketAddress) const;

        /**
         * \brief Sends up to a requested number of bytes through a connected socket.
         *
         * \pre This socket is valid.
         * \pre \p data is not null and \p length is non-zero.
         * \pre \p length fits in the signed length type accepted by the native API.
         *
         * \param data Bytes to send.
         * \param length Number of bytes to send.
         *
         * \return The number of bytes sent, which can be less than \p length,
         *         or an operating system error.
         */
        Result<unsigned int, SocketErrorCode> Send(const void* data, unsigned int length) const;

        /**
         * \brief Sends a datagram to a numeric IP address and port.
         *
         * \pre This socket is valid.
         * \pre \p data is not null and \p length is non-zero.
         * \pre \p length fits in the signed length type accepted by the native API.
         *
         * \param data Bytes to send.
         * \param length Number of bytes to send.
         * \param address Numeric IPv4 or IPv6 destination address.
         * \param port Destination port in host byte order.
         *
         * \return The number of bytes accepted by the native API, or the
         *         operating system error.
         */
        Result<unsigned int, SocketErrorCode> SendTo(const void* data, unsigned int length, const IpAddress& address, unsigned short port) const;

        /**
         * \brief Writes the value of a socket option.
         *
         * The option selects the protocol layer that interprets it and the type
         * of \p value, so an option cannot be written through a type it was not
         * declared with. The value is converted to the layout the native API
         * expects before the call.
         *
         * Whether an option may be written depends on the operating system and
         * on the protocol of the socket. An option that is read-only,
         * unsupported by the platform or inapplicable to this socket reports a
         * failure rather than being ignored, and several options only take
         * effect while the socket is in a particular state, such as before a
         * bind or before a connect. See SocketOptions for the constraints
         * attached to each option.
         *
         * Because \p T is deduced from both parameters, \p value has to have the
         * exact type the option declares.
         *
         * \pre This socket is valid.
         *
         * \tparam T Value type declared by \p option.
         *
         * \param option Option to write.
         * \param value Value to assign to the option.
         *
         * \return A successful Result, or the operating system error. The option
         *         is left unchanged on failure.
         */
        template <typename T>
        Result<void, SocketErrorCode> SetOption(SocketOption<T> option, T value) const
        {
            using Traits = Detail::SocketOptionTraits<T>;
            using NativeType = typename Traits::NativeType;

            NativeType nativeValue = Traits::ToNative(value);

            return SetOption(option.GetLevel(), option.GetId(), &nativeValue, sizeof(NativeType));
        }

        /**
         * \brief Reads the value of a socket option.
         *
         * The option selects the protocol layer that interprets it and the type
         * of the returned value. The value reported by the native API is
         * converted back to that type.
         *
         * A successful read does not mean the value matches what was last
         * written: buffer sizes and timeouts in particular may have been
         * rounded or clamped by the operating system, and some options report a
         * state the socket reached on its own. Reading SocketOptions::Error also
         * clears the error it reports.
         *
         * \pre This socket is valid.
         *
         * \tparam T Value type declared by \p option.
         *
         * \param option Option to read.
         *
         * \return The current value of the option, or the operating system
         *         error.
         */
        template <typename T>
        Result<T, SocketErrorCode> GetOption(const SocketOption<T>& option) const
        {
            using Traits = Detail::SocketOptionTraits<T>;
            using NativeType = typename Traits::NativeType;

            NativeType value{};
            std::size_t valueSize = sizeof(NativeType);

            Result<void, SocketErrorCode> result = GetOption(option.GetLevel(), option.GetId(), &value, valueSize);
            if (result.IsFailure())
                return Result<T, SocketErrorCode>::Failure(result.GetError());

            assert(valueSize == sizeof(NativeType));

            return Result<T, SocketErrorCode>::Success(Traits::FromNative(value));
        }

        /**
         * \brief Gets the numeric endpoint currently assigned to this socket.
         *
         * An unbound socket normally reports the wildcard address selected by
         * its protocol and port zero. A socket bound with port zero reports the
         * ephemeral port selected by the operating system.
         *
         * \pre This socket is valid.
         *
         * \param socketAddress Receives a copy of the local endpoint on success.
         *
         * \return A successful Result, or the operating system error.
         *         \p socketAddress is unchanged on failure.
         */
        Result<void, SocketErrorCode> GetLocalAddress(SocketAddress& socketAddress) const;

        /**
         * \brief Checks whether this instance owns a native socket handle.
         *
         * \return \c true if the socket is valid, \c false otherwise.
         */
        bool IsValid() const;

        /**
         * \brief Checks whether socket operations are configured to block.
         *
         * \return \c true for blocking mode, \c false for non-blocking mode.
         */
        bool IsBlocking() const;

        /**
         * \brief Requests blocking or non-blocking I/O for this socket.
         *
         * \pre This socket is valid.
         *
         * \param enable \c true to block until an operation can proceed, or
         *        \c false to return WouldBlock instead of waiting.
         *
         * \throws SystemException If the operating system rejects the mode change.
         */
        void SetBlocking(bool enable);

        /**
         * \brief Gets the number of bytes immediately readable from the socket.
         *
         * The returned value is a snapshot of the receive buffer. It can change
         * immediately and a value of zero does not by itself identify a remote
         * shutdown.
         *
         * \pre This socket is valid.
         *
         * \return Number of bytes currently reported by the native receive queue.
         *
         * \throws SystemException If the native query fails.
         */
        unsigned int GetPendingLength() const;

        /**
         * \brief Gets the borrowed native handle owned by this socket.
         *
         * \return The native handle, or InvalidHandle when this socket is invalid.
         */
        Handle GetSystemHandle() const;

    private:

        /**
         * \brief Takes ownership of an already-created native socket handle.
         *
         * \pre \p handle is valid.
         *
         * \param handle Native handle whose ownership is transferred here.
         * \param blocking Whether the native socket is configured for blocking I/O.
         */
        Socket(Handle handle, bool blocking);

        /**
         * \brief Reads an option into a caller-provided buffer.
         *
         * Carries the untyped part of the option API, which keeps the
         * translation to native option values out of this header. The public
         * GetOption builds the buffer and converts the result.
         *
         * \pre This socket is valid.
         * \pre \p value points to at least \p size writable bytes.
         *
         * \param level Protocol layer that interprets the option.
         * \param option Option to read within that layer.
         * \param value Buffer receiving the native value.
         * \param size On entry, the capacity of \p value in bytes. On success,
         *        receives the number of bytes the native API reports having
         *        written, which is smaller than the capacity when the option
         *        holds less data than the buffer offered. \p size is unchanged
         *        on failure.
         *
         * \return A successful Result, or the operating system error.
         */
        Result<void, SocketErrorCode> GetOption(SocketOptionLevel level, SocketOptionId option, void* value, std::size_t& size) const;

        /**
         * \brief Writes an option from a caller-provided buffer.
         *
         * Carries the untyped part of the option API. The public SetOption
         * converts the value and supplies its native size.
         *
         * \pre This socket is valid.
         * \pre \p value points to at least \p size readable bytes.
         * \pre \p size matches the layout the native API expects for \p option.
         *
         * \param level Protocol layer that interprets the option.
         * \param option Option to write within that layer.
         * \param value Buffer holding the native value.
         * \param size Size of \p value in bytes.
         *
         * \return A successful Result, or the operating system error.
         */
        Result<void, SocketErrorCode> SetOption(SocketOptionLevel level, SocketOptionId option, const void* value, std::size_t size) const;

        Handle mHandle;
        bool mBlocking;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKET_HPP
