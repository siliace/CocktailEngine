#ifndef FTP_FILESYSTEMDRIVER_FILESYSTEMSERVER_HPP
#define FTP_FILESYSTEMDRIVER_FILESYSTEMSERVER_HPP

#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>
#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/System/Network/TcpServer.hpp>

#include <Ftp/FileSystemDriver/Export.hpp>

#include "CocktailEngine/Core/System/Concurrency/ThreadPool.hpp"

namespace Ck
{
    namespace Ftp
    {
        class FTP_FILESYSTEMDRIVER_API FileSystemServer
        {
        public:

            explicit FileSystemServer(FileSystemDriver* driver);

        private:

            FileSystemDriver* mDriver;
            UniquePtr<Thread> mAcceptThread;
            UniquePtr<Runnable> mAcceptRunnable;
            TcpServer mServer;
            Mutex mClientsMutex;
            Array<TcpClient> mClients;
            UniquePtr<ThreadPool> mThreadPool;
        };
    }

    using FtpFileSystemServer = Ftp::FileSystemServer;
}

#endif // FTP_FILESYSTEMDRIVER_FILESYSTEMSERVER_HPP
