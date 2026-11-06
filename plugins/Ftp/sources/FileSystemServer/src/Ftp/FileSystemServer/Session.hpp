#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMSERVER_SESSION_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMSERVER_SESSION_HPP

#include <atomic>

#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>
#include <CocktailEngine/Core/System/Network/TcpListener.hpp>

#include <Ftp/Core/Server.hpp>

#include <Ftp/FileSystemServer/FileSystemServer.hpp>

namespace Ck::Ftp
{
    /**
     * \brief What a session needs from the server owning it
     */
    struct SessionContext
    {
        Ck::FileSystemDriver* Driver;
        Mutex* DriverMutex;
        const FileSystemServerOptions* Options;
    };

    /**
     * \brief A client connected to a FileSystemServer
     *
     * A session runs on its own thread, it reads the commands of the client
     * until it quits, is disconnected or is stopped.
     */
    class Session : public StoppableRunnable
    {
    public:

        Session(const SessionContext& context, Connection connection);

        /**
         * \brief Start the thread serving the client
         */
        void Start();

        /**
         * \brief Wait for the thread serving the client to end
         */
        void Join();

        /**
         * \brief Tell whether the client has been fully served
         */
        bool IsFinished() const;

        /**
         * \brief Reply to the client that the server can not serve it, then end the session
         *
         * Must be called before Start.
         */
        void Reject(unsigned int code, const Utf8Char* message);

        unsigned int Run() override;

    private:

        struct ListEntry
        {
            Utf8String Name;
            PathInfo Info;
        };

        void Reply(unsigned int code, const Utf8Char* message);
        void Reply(unsigned int code, Utf8String message);

        bool Handle(const Request& request);

        // Commands
        void HandleUser(const Utf8StringView& argument);
        void HandlePass(const Utf8StringView& argument);
        void HandleType(const Utf8StringView& argument);
        void HandleOpts(const Utf8StringView& argument);
        void HandleCwd(const Utf8StringView& argument);
        void HandlePwd();
        void HandleMkd(const Utf8StringView& argument);
        void HandleRmd(const Utf8StringView& argument);
        void HandleDele(const Utf8StringView& argument);
        void HandleRnfr(const Utf8StringView& argument);
        void HandleRnto(const Utf8StringView& argument);
        void HandleSize(const Utf8StringView& argument);
        void HandleMdtm(const Utf8StringView& argument);
        void HandleRest(const Utf8StringView& argument);
        void HandlePasv();
        void HandleEpsv(const Utf8StringView& argument);
        void HandlePort(const Utf8StringView& argument);
        void HandleEprt(const Utf8StringView& argument);
        void HandleList(const Utf8StringView& argument, Request::Code code);
        void HandleMlst(const Utf8StringView& argument);
        void HandleRetr(const Utf8StringView& argument);
        void HandleStore(const Utf8StringView& argument, bool append);

        // Paths
        bool Resolve(const Utf8StringView& argument, Array<String>& segments) const;
        Path ToRealPath(const Array<String>& segments) const;
        Utf8String ToVirtualPath(const Array<String>& segments) const;
        bool CheckWritable();

        // Listings
        bool CollectListing(const Utf8StringView& argument, bool allowFile, Array<ListEntry>& entries, bool& isDirectory);

        // Data connection
        bool HasDataEndpoint() const;
        void ResetDataEndpoint();
        bool ConnectData(TcpClient& data);
        void SendData(const TcpClient& data, const Utf8String& content);

        SessionContext mContext;
        Connection mConnection;
        UniquePtr<Thread> mThread;
        std::atomic<bool> mFinished;

        bool mAuthenticated;
        bool mHasUser;
        Utf8String mUser;
        unsigned int mFailedLogins;

        Array<String> mCurrentDirectory;
        bool mHasRenameSource;
        Path mRenameSource;
        PathType mRenameSourceType;
        Uint64 mRestartOffset;
        bool mQuit;

        UniquePtr<TcpListener> mPassiveListener;
        bool mActiveEnabled;
        Uint8 mActiveAddress[4];
        unsigned short mActivePort;
    };
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMSERVER_SESSION_HPP
