#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck::Detail
{
	class SlotContainer;

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API SlotState : public Extends<SlotState, Object>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		bool Disconnect();

		/**
		 * \brief
		 */
		void Enable();

		/**
		 * \brief 
		 */
		void Disable();

		/**
		 * \brief 
		 * \return 
		 */
		bool IsConnected() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsDisabled();

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetGroupId() const;

	protected:

		/**
		 * \brief 
		 * \param container
		 * \param groupId 
		 */
		SlotState(SlotContainer* container, unsigned int groupId);

	private:

		SlotContainer* mContainer;
		unsigned int mGroupId;
		std::atomic<bool> mConnected;
		std::atomic<bool> mDisabled;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP
