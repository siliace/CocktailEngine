#include <Cocktail/Core/Log/Log.hpp>

namespace Ck
{
	void Log::RegisterChannel(const String& name, std::unique_ptr<LogChannel> logChannel)
	{
		ResolveFacadeInstance()->RegisterChannel(name, std::move(logChannel));
	}
}
