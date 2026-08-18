#include <CocktailEngine/Core/System/Monitor/MonitorTopologyHelper.hpp>

namespace Ck
{
    MonitorTopologyHelper::ChangeFlags MonitorTopologyHelper::Compare(const Array<Entry>& previous, const Array<Entry>& current)
    {
        ChangeFlags difference;

        if (previous.GetSize() != current.GetSize())
            difference |= TopologyChangeFlagBits::Set;

        for (const Entry& currentEntry : current)
        {
            const Entry* previousEntry = FindTopologyEntry(previous, currentEntry.Identifier);
            if (previousEntry == nullptr)
            {
                difference |= TopologyChangeFlagBits::Set;
                continue;
            }

            if (previousEntry->Position != currentEntry.Position || previousEntry->Size != currentEntry.Size)
                difference |= TopologyChangeFlagBits::Geometry;
        }

        // Check removals explicitly because monitor identity, not enumeration order,
        // determines membership in a topology snapshot.
        for (const Entry& previousEntry : previous)
        {
            if (FindTopologyEntry(current, previousEntry.Identifier) == nullptr)
                difference |= TopologyChangeFlagBits::Set;
        }

        bool previousHasPrimaryMonitor = false;
        bool currentHasPrimaryMonitor = false;
        const Uint64 previousPrimaryIdentifier = GetPrimaryMonitorIdentifier(previous, previousHasPrimaryMonitor);
        const Uint64 currentPrimaryIdentifier = GetPrimaryMonitorIdentifier(current, currentHasPrimaryMonitor);

        if (previousHasPrimaryMonitor != currentHasPrimaryMonitor || (previousHasPrimaryMonitor && previousPrimaryIdentifier != currentPrimaryIdentifier))
            difference |= TopologyChangeFlagBits::Primary;

        return difference;
    }
    const MonitorTopologyHelper::Entry* MonitorTopologyHelper::FindTopologyEntry(const Array<Entry>& topology, Uint64 identifier)
    {
        for (const Entry& entry : topology)
        {
            if (entry.Identifier == identifier)
                return &entry;
        }

        return nullptr;
    }
    Uint64 MonitorTopologyHelper::GetPrimaryMonitorIdentifier(const Array<Entry>& topology, bool& hasPrimaryMonitor)
    {
        hasPrimaryMonitor = false;

        for (const Entry& entry : topology)
        {
            if (!entry.IsPrimary)
                continue;

            hasPrimaryMonitor = true;
            return entry.Identifier;
        }

        return 0;
    }
}
