#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_AFFINITYMASKBUILDER_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_AFFINITYMASKBUILDER_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    /**
     * \brief Utility class for building CPU affinity masks
     *
     * This class provides a convenient, fluent API for specifying
     * which CPU cores a thread is allowed to run on
     */
    class COCKTAIL_CORE_API AffinityMaskBuilder
    {
    public:

        /**
         * \brief Creates a mask including all available CPU cores
         * \return A builder with all cores included
         */
        static AffinityMaskBuilder All();

        /**
         * \brief Constructs an empty affinity mask
         */
        AffinityMaskBuilder();

        /**
         * \brief Includes a single CPU core in the mask
         * \param cpuIndex Index of the CPU core to include
         * \return Reference to this builder
         */
        AffinityMaskBuilder& Include(unsigned int cpuIndex);

        /**
         * \brief Includes a range of CPU cores in the mask
         * \param firstIndex Index of the first CPU core
         * \param lastIndex Index of the last CPU core
         * \return Reference to this builder
         */
        AffinityMaskBuilder& Include(unsigned int firstIndex, unsigned int lastIndex);

        /**
         * \brief Checks if a specific CPU core is included in the mask
         * \param cpuIndex Index of the CPU core to check
         * \return True if the core is included, false otherwise
         */
        bool HasCpu(unsigned int cpuIndex) const;

        /**
         * \brief Excludes a single CPU core from the mask
         * \param cpuIndex Index of the CPU core to exclude
         * \return Reference to this builder
         */
        AffinityMaskBuilder& Exclude(unsigned int cpuIndex);

        /**
         * \brief Excludes a range of CPU cores from the mask
         * \param firstIndex Index of the first CPU core
         * \param lastIndex Index of the last CPU core
         * \return Reference to this builder
         */
        AffinityMaskBuilder& Exclude(unsigned int firstIndex, unsigned int lastIndex);

        /**
         * \brief Clears the mask, removing all CPU cores
         */
        void Clear();

        /**
         * \brief Returns the constructed affinity mask as a bitmask
         * \return Affinity bitmask
         */
        Uint64 GetAffinityMask() const;

    private:

        Uint64 mMask; /*! Internal bitmask representing included CPU cores */
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_AFFINITYMASKBUILDER_HPP
