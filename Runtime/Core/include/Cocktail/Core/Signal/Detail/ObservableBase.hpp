#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP

#include <vector>
#include <mutex>

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
		ObservableBase(const ObservableBase<Lockable>& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 */
		ObservableBase(ObservableBase<Lockable>&& other) noexcept
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
		ObservableBase<Lockable>& operator=(const ObservableBase<Lockable>& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		ObservableBase<Lockable>& operator=(ObservableBase<Lockable>&& other) noexcept
		{
			std::lock_guard<Lockable> lhs(mMutex);
			std::lock_guard<Lockable> rhs(other.mMutex);

			mConnections = std::move(other.mConnections);

			return *this;
		}

		/** 
		 * \brief
		 * \param signal
		 * \param callback
		 */
		template <typename SignalLockable, typename... Args, typename Callback>
		void Connect(SignalBase<SignalLockable, Args...>& signal, Callback&& callback)
		{
			std::lock_guard<Lockable> lg(mMutex);
			Connection connection = signal.Connect(std::forward<Callback>(callback));
			mConnections.emplace_back(std::move(connection));
		}

	protected:

		/**
		 * \brief 
		 */
		void DisconnectAll()
		{
			std::lock_guard<Lockable> lg(mMutex);
			mConnections.clear();
		}

	private:

		Lockable mMutex;
		std::vector<Connection> mConnections;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
