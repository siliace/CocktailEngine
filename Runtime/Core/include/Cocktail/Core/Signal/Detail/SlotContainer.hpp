#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_SLOTCONTAINER_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_SLOTCONTAINER_HPP

namespace Ck::Detail
{
	class SlotState;

	/**
	 * \brief Base interface for Signal to handle Slot disconnection
	 */
	class SlotContainer
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~SlotContainer() = default;

		/**
		 * \brief Disconnect every Slot from the Signal
		 */
		virtual void Disconnect() = 0;

		/**
		 * \brief Disconnect a whole group from the Signal
		 * \param groupId The group to disconnect
		 */
		virtual void Disconnect(unsigned int groupId) = 0;

	protected:

		friend class SlotState;

		/**
		 * \brief Disconnect a Slot from the Signal
		 * \param state The Slot to disconnect
		 */
		virtual void Disconnect(SlotState* state) = 0;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SLOTCONTAINER_HPP
