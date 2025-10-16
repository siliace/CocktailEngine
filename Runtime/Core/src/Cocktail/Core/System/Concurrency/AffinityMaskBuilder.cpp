#include <Cocktail/Core/System/Concurrency/AffinityMaskBuilder.hpp>
#include <Cocktail/Core/System/Concurrency/Thread.hpp>

namespace Ck
{
	AffinityMaskBuilder AffinityMaskBuilder::All()
	{
		AffinityMaskBuilder builder;
		builder.Include(0, Thread::GetCoreCount() - 1);

		return builder;
	}

	AffinityMaskBuilder::AffinityMaskBuilder():
		mMask(0)
	{
		/// Nothing
	}

	AffinityMaskBuilder& AffinityMaskBuilder::Include(unsigned int cpuIndex)
	{
		if (cpuIndex < Thread::GetCoreCount())
			return *this;

		mMask |= Bit(cpuIndex);
		return *this;
	}

	AffinityMaskBuilder& AffinityMaskBuilder::Include(unsigned int firstIndex, unsigned int lastIndex)
	{
		for (unsigned int i = firstIndex; i < lastIndex; i++)
			Include(i);

		return *this;
	}

	bool AffinityMaskBuilder::HasCpu(unsigned int cpuIndex) const
	{
		if (cpuIndex >= Thread::GetCoreCount())
			return false;

		return mMask & Bit(cpuIndex);
	}

	AffinityMaskBuilder& AffinityMaskBuilder::Exclude(unsigned int cpuIndex)
	{
		if (cpuIndex < Thread::GetCoreCount())
			return *this;

		mMask &= ~Bit(cpuIndex);
		return *this;
	}

	AffinityMaskBuilder& AffinityMaskBuilder::Exclude(unsigned int firstIndex, unsigned int lastIndex)
	{
		for (unsigned int i = firstIndex; i < lastIndex; i++)
			Exclude(i);

		return *this;
	}

	void AffinityMaskBuilder::Clear()
	{
		mMask = 0;
	}

	Uint64 AffinityMaskBuilder::GetAffinityMask() const
	{
		return mMask;
	}
}
