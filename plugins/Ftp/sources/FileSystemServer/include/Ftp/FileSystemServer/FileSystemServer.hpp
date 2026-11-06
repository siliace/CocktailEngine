#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMSERVER_FILESYSTEMSERVER_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMSERVER_FILESYSTEMSERVER_HPP

#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

#include <Ftp/Core/Server.hpp>

#include <Ftp/FileSystemServer/Export.hpp>

namespace Ck
{
    namespace Ftp
    {
        class Session;

        /**
         * \brief A user allowed to log in on a \ref FileSystemServer
         */
        struct Account
        {
            Utf8String User;
            Utf8String Password;
        };

        /**
         * \brief Settings of a \ref FileSystemServer
         */
        struct FileSystemServerOptions
        {
            /**
             * \brief Port the control connections are accepted on
             */
            unsigned short Port = 21;

            /**
             * \brief Directory of the driver exposed as the root ("/") of the server
             *
             * Clients can not leave this directory, the paths they send are resolved
             * against it after the "." and ".." elements have been removed.
             */
            Path Root;

            /**
             * \brief Refuse every command modifying the file system
             */
            bool ReadOnly = false;

            /**
             * \brief Allow the "anonymous" and "ftp" users, whatever password they provide
             */
            bool AllowAnonymous = false;

            /**
             * \brief Users allowed to log in
             *
             * When there is no account and AllowAnonymous is false, nobody can log in.
             */
            Array<Account> Accounts;

            /**
             * \brief Maximum number of simultaneous control connections
             */
            unsigned int MaxConnections = 64;

            /**
             * \brief Time after which a client sending nothing is disconnected
             */
            Duration IdleTimeout = Duration::Minutes(5);
        };

        /**
         * \brief FTP server exposing the content of any FileSystemDriver
         *
         * The server starts accepting connections as soon as it is constructed and
         * stops when it is destroyed. Each client is served by its own thread,
         * every access to the driver is serialized by the server so the driver does
         * not have to be thread-safe.
         *
         * The supported commands are the ones of RFC 959 needed to browse and
         * transfer files in stream mode (passive and active), plus the extensions
         * FEAT, OPTS, SIZE, MDTM, REST STREAM, EPSV, EPRT, MLST and MLSD.
         * The data connections are not encrypted.
         */
        class FTP_FILESYSTEMSERVER_API FileSystemServer
        {
        public:

            /**
             * \param driver The driver whose content is exposed, it must outlive the server
             * \param options The settings of the server
             * \param host The local address the control connections are accepted on
             *
             * \throws SocketException If the port can not be bound
             */
            explicit FileSystemServer(Ck::FileSystemDriver* driver, FileSystemServerOptions options = {}, const IpAddress& host = IpAddressV4::Any);
            ~FileSystemServer();

            FileSystemServer(const FileSystemServer&) = delete;
            FileSystemServer& operator=(const FileSystemServer&) = delete;

            /**
             * \brief Stop accepting connections and disconnect every client
             *
             * Called by the destructor, calling it more than once is harmless.
             */
            void Stop();

            /**
             * \brief Retrieve the number of clients currently connected
             */
            unsigned int GetConnectionCount();

        private:

            void AcceptLoop(StoppableRunnable* runnable);
            void ReapSessions();

            Ck::FileSystemDriver* mDriver;
            FileSystemServerOptions mOptions;
            Mutex mDriverMutex;
            Server mServer;
            UniquePtr<StoppableRunnable> mAcceptRunnable;
            UniquePtr<Thread> mAcceptThread;
            Mutex mSessionsMutex;
            Array<UniquePtr<Session>> mSessions;
        };
    }

    using FtpAccount = Ftp::Account;
    using FtpFileSystemServerOptions = Ftp::FileSystemServerOptions;
    using FtpFileSystemServer = Ftp::FileSystemServer;
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMSERVER_FILESYSTEMSERVER_HPP
