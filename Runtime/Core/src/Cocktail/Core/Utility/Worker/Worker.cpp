#include <Cocktail/Core/System/Concurrency/ThreadUtils.hpp>
#include <Cocktail/Core/Utility/Worker/Worker.hpp>

namespace Ck
{
	Worker::Worker(WorkerGroup* group, Ref<WorkerJob> job):
		mGroup(group),
		mJob(std::move(job)),
		mRunning(false)
	{
		mThread = std::thread(&Worker::Loop, this);
		mRunning = true;
	}

	Worker::~Worker()
	{
		Terminate();
	}

	void Worker::Terminate()
	{
		if (mRunning)
		{
			mRunning = false;
			mThread.join();
		}
	}

	WorkerGroup* Worker::GetGroup() const
	{
		return mGroup;
	}

	void Worker::SetAffinityMask(unsigned affinityMask)
	{
		ThreadUtils::SetAffinity(mThread.native_handle(), affinityMask);
	}

	const std::string& Worker::GetName() const
	{
		return mName;
	}

	void Worker::SetName(std::string name)
	{
		mName = std::move(name);
		ThreadUtils::SetName(mThread.native_handle(), mName);
	}

	void Worker::Loop()
	{
		try
		{
			mJob->Start(this);

			while (mRunning)
				mJob->Cycle(this);

			mJob->Terminate(this);
		}
		catch (const std::exception& exception)
		{
			mJob->Catch(this, exception);
		}
	}
}
