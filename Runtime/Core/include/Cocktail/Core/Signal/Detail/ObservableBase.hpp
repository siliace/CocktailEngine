#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP

#include <mutex>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Signal/Connection.hpp>

namespace Ck::Detail
{
	/**
	 * \brief 
	 */
	template <typename Lockable>
	class COCKTAIL_CORE_API ObservableBase
	{
	public:

		/**
	     * \brief 
	     */
		ObservableBase() = default;

		/**
		 * \brief 
		 * \param other 
		 */
		ObservableBase(const ObservableBase& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 */
		ObservableBase(ObservableBase&& other) noexcept
		{
			std::lock_guard<Lockable> lhs(mMutex);
			std::lock_guard<Lockable> rhs(other.mMutex);

			mConnections = std::move(other.mConnections);
		}

		/**
		 * \brief
		 */
		virtual ~ObservableBase() = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		ObservableBase& operator=(const ObservableBase& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		ObservableBase& operator=(ObservableBase&& other) noexcept
		{
		    if (this != &other)
		    {
		        std::lock_guard<Lockable> lhs(mMutex);
		        std::lock_guard<Lockable> rhs(other.mMutex);

		        mConnections = std::move(other.mConnections);
		    }

			return *this;
		}

		/** 
		 * \brief
		 * \param signal
		 * \param callback
		 */
		template <typename SignalLockable, typename... Args, typename Callback>
		Connection& Connect(SignalBase<SignalLockable, Args...>& signal, Callback&& callback)
		{
			std::lock_guard<Lockable> lg(mMutex);

			Connection connection = signal.Connect(std::forward<Callback>(callback));
			mConnections.Add(std::move(connection));

		    return mConnections.Last();
		}

	protected:

		/**
		 * \brief 
		 */
		void DisconnectAll()
		{
			std::lock_guard<Lockable> lg(mMutex);
			mConnections.Clear();
		}

	private:

		Lockable mMutex;
		Array<Connection> mConnections;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
