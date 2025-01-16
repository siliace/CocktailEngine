#ifndef COCKTAIL_CORE_SIGNAL_CONNECTION_HPP
#define COCKTAIL_CORE_SIGNAL_CONNECTION_HPP

#include <memory>

#include <Cocktail/Core/Signal/Detail/SlotState.hpp>

namespace Ck
{
	namespace Detail
	{
		template <typename, typename...>
		class SignalBase;
	}

	/**
	 * \brief A connection object allows interaction with an ongoing Slot connection
	 * It performs common actions on a Slot like connection, disabling and disconnection
	 * Connection is a RAII object so the underlying slot will be disconnected when the object is destroyed
	 */
	class COCKTAIL_CORE_API Connection
	{
	public:

		/**
		 * \brief Default constructor
		 * Create a connection without underlying slot
		 */
		Connection() = default;

		/**
		 * \brief Deleted copy constructor
		 */
		Connection(const Connection&) = delete;

		/**
		 * \brief Default move constructor
		 * \param other 
		 */
		Connection(Connection&& other) noexcept = default;

		/** 
		 * \brief Disconnect the underlying Slot
		 */
		~Connection();

		/**
		 * \brief 
		 * \return 
		 */
		Connection& operator=(const Connection&) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Connection& operator=(Connection&& other) noexcept = default;

		/**
		 * \brief Disconnect the underlying Slot from the Signal where its connected 
		 */
		void Disconnect();

		/**
		 * \brief Enable the Slot
		 * This call make the Slot being invoked when the Signal where its connected is emitted
		 */
		void Enable() const;

		/**
		 * \brief Disable the Slot
	     * This call make the Slot being not invoked when the Signal where its connected is emitted
		 */
		void Disable() const;

		/**
		 * \brief Tell whether the underlying Slot is still connected to the Signal it come from
		 * \return True if the Slot is still connected, false otherwise
		 */
		bool IsConnected() const;

		/**
		 * \brief Tell whether the underlying Slot is disabled
		 * \return True if the Slot is disabled, false otherwise
		 */
		bool IsDisabled() const;

	private:

		template <typename, typename...>
		friend class Detail::SignalBase;

		/**
		 * \brief Constructor
		 * \param state The Slot to manage
		 */
		explicit Connection(std::weak_ptr<Detail::SlotState> state);

		std::weak_ptr<Detail::SlotState> mState;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_CONNECTION_HPP
