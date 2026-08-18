#include <Ftp/FileSystemDriver/FileSystemServer.hpp>

#include "CocktailEngine/Core/System/Concurrency/Runnable.hpp"

namespace Ck::Ftp
{
    FileSystemServer::FileSystemServer(FileSystemDriver* driver) :
        mDriver(driver),
        mServer(21, IpAddressV4::Any)
    {
        mAcceptRunnable = MakeStoppableRunnable([this](StoppableRunnable* runnable) {
            mServer.EnableBlockingMode(false);
            while (!runnable->ShouldStop())
            {
                mServer.TryAccept(Duration::Seconds(5), Duration::Milliseconds(100)).Then([&](TcpClient& client) {
                    LockGuard<Mutex> lock(mClientsMutex);
                    mClients.Add(Move(client));
                });
            }
        });

        mThreadPool = MakeUnique<ThreadPool>(Thread::GetCoreCount(), CK_TEXT("FtpRequestWorker"));
    }
}
