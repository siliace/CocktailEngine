#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Signal/Connection.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>

namespace Ck::Detail
{
	/**
	 * \brief 
	 */
	template <Lockable TLockable>
	class COCKTAILENGINE_CORE_API ObservableBase
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
			LockGuard lhs(mMutex);
			LockGuard rhs(other.mMutex);

			mConnections = Move(other.mConnections);
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
		        LockGuard lhs(mMutex);
		        LockGuard rhs(other.mMutex);

		        mConnections = Move(other.mConnections);
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
			LockGuard lg(mMutex);

			Connection connection = signal.Connect(Forward<Callback>(callback));
			mConnections.Add(Move(connection));

		    return mConnections.Last();
		}

	protected:

		/**
		 * \brief 
		 */
		void DisconnectAll()
		{
			LockGuard lg(mMutex);
			mConnections.Clear();
		}

	private:

		TLockable mMutex;
		Array<Connection> mConnections;
	};
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
