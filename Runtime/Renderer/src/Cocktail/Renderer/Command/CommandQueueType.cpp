#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

namespace Ck::Renderer
{
	CommandQueueType SelectQueueForUsage(CommandListUsage usage)
	{
		if (usage & CommandListUsageBits::Graphic)
			return CommandQueueType::Graphic;

		if (usage & CommandListUsageBits::Compute)
			return CommandQueueType::Compute;

		return CommandQueueType::Transfer;
	}
}
