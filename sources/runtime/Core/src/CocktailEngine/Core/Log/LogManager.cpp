#include <CocktailEngine/Core/Log/LogManager.hpp>
#include <CocktailEngine/Core/Memory/Ptr.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>

namespace Ck
{
    LogManager::LogManager(ThreadManager* threadManager)
    {
        mPendingEntriesRunnable = MakeStoppableRunnable([this](StoppableRunnable* runnable) {
            while (!runnable->ShouldStop())
            {
                mPendingEntries.TakeUntil([&]() {
                    return runnable->ShouldStop();
                }).Then([&](LogEntry* logEntry) {
                    for (const auto& [name, channel] : mChannels)
                        channel->WriteEntry(*logEntry);
                });
            }
        });

        mPendingEntriesThread = threadManager->Create(mPendingEntriesRunnable.Get(), CK_TEXT("LogDispatcher"), 0);
        mPendingEntriesThread->SetPriority(Thread::Priority::SlightlyBelowNormal);
    }

    LogManager::~LogManager()
    {
        mPendingEntriesRunnable->Stop();
        mPendingEntries.Close();
        mPendingEntriesThread->Join();
    }

    void LogManager::RegisterChannel(const String& name, UniquePtr<LogChannel> logChannel)
    {
        mChannels.Put(name, Move(logChannel));
    }

    void LogManager::GetEntries(Array<LogEntry*>& entries) const
    {
        entries.Clear();
        for (Ptr<LogEntry> entry : mEntries)
            entries.Add(entry.Get());
    }

    Signal<LogEntry*>& LogManager::OnTraceEntry()
    {
        return mOnTraceEntry;
    }
}
