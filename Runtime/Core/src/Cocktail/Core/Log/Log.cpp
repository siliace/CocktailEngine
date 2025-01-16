#include <Cocktail/Core/Log/Log.hpp>

namespace Ck
{
	void Log::RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel)
	{
		ResolveFacadeInstance()->RegisterChannel(name, std::move(logChannel));
	}

	LogLevel Log::GetLevel()
	{
		return ResolveFacadeInstance()->GetLevel();
	}

	void Log::SetLevel(LogLevel level)
	{
		ResolveFacadeInstance()->SetLevel(level);
	}
}
