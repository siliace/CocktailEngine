#include <Cocktail/Core/Log/Log.hpp>

namespace Ck
{
	void Log::RegisterChannel(const String& name, UniquePtr<LogChannel> logChannel)
	{
		ResolveFacadeInstance()->RegisterChannel(name, std::move(logChannel));
	}
}
