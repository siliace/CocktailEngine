#ifndef COCKTAILENGINE_CORE_SYSTEM_MONITOR_TOPOLOGY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_MONITOR_TOPOLOGY_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Extent2D.hpp>
#include <CocktailEngine/Core/Flags.hpp>

namespace Ck
{
    class MonitorTopologyHelper
    {
    public:

        enum class TopologyChangeFlagBits
        {
            Set,
            Primary,
            Geometry
        };

        using ChangeFlags = Flags<TopologyChangeFlagBits>;

        /**
         * \brief Platform-independent monitor state used to compare display snapshots.
         */
        struct Entry
        {
            Uint64 Identifier = 0; /*!< Stable identity supplied by the platform backend for this snapshot */
            bool IsPrimary = false; /*!< Whether this monitor is the primary display */
            Extent2D<int> Position; /*!< Monitor origin in the virtual desktop, in pixels */
            Extent2D<unsigned int> Size; /*!< Monitor resolution in pixels */
        };

        /**
         * \brief Compares monitor snapshots independently of their enumeration order.
         *
         * The comparison detects monitor additions, removals, primary-monitor changes,
         * and changes to the position or resolution of an existing monitor.
         *
         * \param previous Previously published monitor snapshot.
         * \param current Newly enumerated monitor snapshot.
         * \return The difference between both snapshots.
         */
        static ChangeFlags Compare(const Array<Entry>& previous, const Array<Entry>& current);

    private:

        static const Entry* FindTopologyEntry(const Array<Entry>& topology, Uint64 identifier);

        static Uint64 GetPrimaryMonitorIdentifier(const Array<Entry>& topology, bool& hasPrimaryMonitor);
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_MONITOR_TOPOLOGY_HPP
