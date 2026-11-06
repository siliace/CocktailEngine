#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>

#include <Ftp/FileSystemServer/FileSystemServer.hpp>
#include <Ftp/FileSystemServer/Session.hpp>

namespace Ck::Ftp
{
    FileSystemServer::FileSystemServer(Ck::FileSystemDriver* driver, FileSystemServerOptions options, const IpAddress& host) :
        mDriver(driver),
        mOptions(Move(options)),
        mServer(mOptions.Port, host)
    {
        assert(mDriver != nullptr);

        mAcceptRunnable = MakeStoppableRunnable(*this, &FileSystemServer::AcceptLoop);
        mAcceptThread = Thread::Create(mAcceptRunnable.Get(), CK_TEXT("FtpAccept"));
    }

    FileSystemServer::~FileSystemServer()
    {
        Stop();
    }

    void FileSystemServer::Stop()
    {
        if (mAcceptThread)
        {
            mAcceptRunnable->Stop();
            mAcceptThread->Join();
            mAcceptThread = UniquePtr<Thread>();
        }

        // The accept thread is gone, nobody adds sessions anymore
        LockGuard<Mutex> lock(mSessionsMutex);

        for (UniquePtr<Session>& session : mSessions)
            session->Stop();

        for (UniquePtr<Session>& session : mSessions)
            session->Join();

        mSessions.Clear();
    }

    unsigned int FileSystemServer::GetConnectionCount()
    {
        LockGuard<Mutex> lock(mSessionsMutex);

        unsigned int count = 0;
        for (const UniquePtr<Session>& session : mSessions)
        {
            if (!session->IsFinished())
                count++;
        }

        return count;
    }

    void FileSystemServer::AcceptLoop(StoppableRunnable* runnable)
    {
        SessionContext context;
        context.Driver = mDriver;
        context.DriverMutex = &mDriverMutex;
        context.Options = &mOptions;

        while (!runnable->ShouldStop())
        {
            ReapSessions();

            try
            {
                Optional<Connection> connection = mServer.TryAccept(Duration::Milliseconds(250), Duration::Milliseconds(10));
                if (connection.IsEmpty())
                    continue;

                UniquePtr<Session> session = MakeUnique<Session>(context, Move(connection.Get()));

                LockGuard<Mutex> lock(mSessionsMutex);

                if (mSessions.GetSize() >= mOptions.MaxConnections)
                {
                    session->Reject(421, CK_UTF8_TEXT("Too many connections, try again later"));
                    continue;
                }

                session->Start();
                mSessions.Add(Move(session));
            }
            catch (const Exception&)
            {
                // A failing accept (e.g. a client gone before being accepted) must not stop the server
                Thread::SleepFor(Duration::Milliseconds(50));
            }
        }
    }

    void FileSystemServer::ReapSessions()
    {
        LockGuard<Mutex> lock(mSessionsMutex);

        for (unsigned int i = 0; i < mSessions.GetSize();)
        {
            if (!mSessions[i]->IsFinished())
            {
                i++;
                continue;
            }

            mSessions[i]->Join();
            mSessions.RemoveAt(i);
        }
    }
}
