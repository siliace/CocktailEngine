#ifndef COCKTAIL_CORE_UTILITY_WORKER_CALLABLETASK_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_CALLABLETASK_HPP

#include <future>
#include <tuple>

#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerTask.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	template <typename Callable, typename... Args>
	class COCKTAIL_CORE_API CallableTask : public Inherit<CallableTask<Callable, Args...>, Object, WorkerTask>
	{
	public:

		using ResultType = std::invoke_result_t<Callable, Args...>;

		/**
		 * \brief 
		 * \param callable 
		 * \param args 
		 */
		explicit CallableTask(Callable&& callable, Args&&... args) :
			mCallable(std::forward<Callable>(callable)),
			mArgs(std::make_tuple(std::forward<Args>(args)...))
		{
			/// Nothing
		}

		/**
		 * \brief 
		 */
		void Process() override
		{
			try
			{
				if constexpr (std::is_void_v<ResultType>)
				{
					std::apply(mCallable, mArgs);
					mPromise.set_value();
				}
				else
				{
					auto result = std::apply(mCallable, mArgs);
					mPromise.set_value(std::move(result));
				}
			}
			catch (...)
			{
				mPromise.set_exception(
					std::current_exception()
				);
			}
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::future<ResultType> ToFuture()
		{
			return mPromise.get_future();
		}

	private:

		std::promise<ResultType> mPromise;
		Callable mCallable;
		std::tuple<Args...> mArgs;
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_CALLABLETASK_HPP
