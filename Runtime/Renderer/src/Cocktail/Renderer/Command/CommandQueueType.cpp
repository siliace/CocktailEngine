#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

namespace Ck::Renderer
{
	CommandQueueType SelectQueueForUsage(const Flags<CommandListUsage>& usage)
	{
		if (usage & CommandListUsage::Graphic)
			return CommandQueueType::Graphic;

		if (usage & CommandListUsage::Compute)
			return CommandQueueType::Compute;

		return CommandQueueType::Transfer;
	}
}
