#ifndef COCKTAIL_CORE_UTILITY_VALUEGUARD_HPP
#define COCKTAIL_CORE_UTILITY_VALUEGUARD_HPP

namespace Ck
{
    /**
     * \brief RAII utility that temporarily modifies a referenced value
     *        and restores its original state upon destruction
     *
     * ValueGuard stores the original value of a referenced object and
     * replaces it with a temporary value during its lifetime.
     * When the guard is destroyed, the original value is automatically restored.
     *
     * This is useful for temporarily overriding a variable in a scoped context
     * (e.g., flags, settings, or configuration values) with exception safety.
     *
     * \tparam ReferenceType Type of the referenced object to be modified
     * \tparam OriginalType  Type used to store the original value
     */
    template <typename ReferenceType, typename OriginalType>
    class ValueGuard
    {
    public:

        /**
         * \brief Constructs the guard and temporarily replaces the referenced value
         *
         * Stores the current value of \p reference and replaces it with \p value.
         *
         * \param reference Reference to the value to be temporarily modified
         * \param value Temporary value to assign to the referenced object
         */
        ValueGuard(ReferenceType& reference, const OriginalType& value) :
            mReference(reference),
            mOriginalValue(reference)
        {
            mReference = value;
        }

        /**
         * \brief Deleted copy constructor to prevent copying
         */
        ValueGuard(const ValueGuard& other) = delete;

        /**
         * \brief Destructor restores the original value
         */
        ~ValueGuard()
        {
            mReference = mOriginalValue;
        }

        /**
         * \brief Deleted copy assignment operator to prevent copying
         */
        ValueGuard& operator=(const ValueGuard& other) = delete;

        /**
         * \brief Retrieves the originally stored value
         *
         * \return Constant reference to the original value saved at construction time
         */
        const OriginalType& GetOriginalValue() const
        {
            return mOriginalValue;
        }

    private:

        ReferenceType& mReference; /*!< Reference to the modified object */
        OriginalType mOriginalValue; /*!< Stored original value restored upon destruction */
    };
}

#endif // COCKTAIL_CORE_UTILITY_VALUEGUARD_HPP
