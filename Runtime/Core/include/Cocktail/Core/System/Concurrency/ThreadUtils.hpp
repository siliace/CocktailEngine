#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADUTILS_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADUTILS_HPP

#include <thread>

#include <Cocktail/Core/StringView.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API ThreadAffinityMaskBuilder
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static ThreadAffinityMaskBuilder All();

		/**
		 * \brief 
		 */
		ThreadAffinityMaskBuilder();

		/**
		 * \brief 
		 * \param cpuIndex 
		 */
		void Include(unsigned int cpuIndex);

		/**
		 * \brief 
		 * \param firstIndex 
		 * \param lastIndex 
		 */
		void Include(unsigned int firstIndex, unsigned int lastIndex);

		/**
		 * \brief 
		 * \param cpuIndex 
		 * \return 
		 */
		bool HasCpu(unsigned int cpuIndex) const;

		/**
		 * \brief 
		 * \param cpuIndex 
		 */
		void Exclude(unsigned int cpuIndex);

		/**
		 * \brief 
		 * \param firstIndex 
		 * \param lastIndex 
		 */
		void Exclude(unsigned int firstIndex, unsigned int lastIndex);

		/**
		 * \brief 
		 */
		void Clear();

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetAffinityMask() const;

	private:

		unsigned int mMask;
	};

	/**
	 * \brief
	 */
	class COCKTAIL_CORE_API ThreadUtils
	{
	public:

		/**
		 * \brief Get the thread handle of the current thread
		 * This function is here to fill the lack of std::this_thread::get_id().native_handle()
		 * \return The current thread handle
		 */
		static std::thread::native_handle_type GetCurrentThreadHandle();

		/**
		 * \brief Set the name of a thread
		 * \param threadHandle
		 * \param name
		 */
		static void SetName(std::thread::native_handle_type threadHandle, StringView name);

		/**
		 * \brief
		 * \param threadHandle
		 * \param affinityMask
		 */
		static void SetAffinity(std::thread::native_handle_type threadHandle, unsigned int affinityMask);

		/**
		 * \brief Make the calling thread to sleep for a given \p duration
		 * \param duration
		 */
		static void SleepFor(const Duration& duration);

		/**
		 * \brief Make the calling thread to sleep until a \p instant has been reached
		 * \param instant 
		 */
		static void SleepUntil(const Instant& instant, const Duration& duration = Duration::Milliseconds(1));

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 * \param duration 
		 */
		template <typename Callable>
		static void WaitUntil(Callable callable, const Duration& duration = Duration::Milliseconds(1))
		{
			while (!callable())
				SleepFor(duration);
		}
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADUTILS_HPP
