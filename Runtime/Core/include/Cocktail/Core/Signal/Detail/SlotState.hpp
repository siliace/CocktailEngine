#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP

#include <atomic>

#include <Cocktail/Core/Export.hpp>

namespace Ck::Detail
{
	class SlotContainer;

    template <typename, typename...>
    class SignalBase;

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API SlotState
	{
	public:

		/*
		 * \brief Destructor
		 */
		virtual ~SlotState() = default;

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

	    template <typename, typename...>
	    friend class SignalBase;

        /**
         * \brief
         *
         * \param container
         */
	    void Rebind(SlotContainer* container);

		SlotContainer* mContainer;
		unsigned int mGroupId;
		std::atomic<bool> mConnected;
		std::atomic<bool> mDisabled;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP
