#include <Cocktail/Core/System/Concurrency/ThreadUtils.hpp>

namespace Ck
{
	ThreadAffinityMaskBuilder ThreadAffinityMaskBuilder::All()
	{
		ThreadAffinityMaskBuilder builder;
		builder.Include(0, std::thread::hardware_concurrency() - 1);

		return builder;
	}

	ThreadAffinityMaskBuilder::ThreadAffinityMaskBuilder():
		mMask(0)
	{
		/// Nothing
	}

	void ThreadAffinityMaskBuilder::Include(unsigned int cpuIndex)
	{
		if (cpuIndex < std::thread::hardware_concurrency())
			return;

		mMask |= Bit(cpuIndex);
	}

	void ThreadAffinityMaskBuilder::Include(unsigned int firstIndex, unsigned int lastIndex)
	{
		for (unsigned int i = firstIndex; i < lastIndex; i++)
			Include(i);
	}

	bool ThreadAffinityMaskBuilder::HasCpu(unsigned int cpuIndex) const
	{
		if (cpuIndex >= std::thread::hardware_concurrency())
			return false;

		return mMask & Bit(cpuIndex);
	}

	void ThreadAffinityMaskBuilder::Exclude(unsigned int cpuIndex)
	{
		if (cpuIndex < std::thread::hardware_concurrency())
			return;

		mMask &= ~Bit(cpuIndex);
	}

	void ThreadAffinityMaskBuilder::Exclude(unsigned int firstIndex, unsigned int lastIndex)
	{
		for (unsigned int i = firstIndex; i < lastIndex; i++)
			Exclude(i);
	}

	void ThreadAffinityMaskBuilder::Clear()
	{
		mMask = 0;
	}

	unsigned int ThreadAffinityMaskBuilder::GetAffinityMask() const
	{
		return mMask;
	}
}
