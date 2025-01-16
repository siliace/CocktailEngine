#include <Cocktail/Core/Log/LogManager.hpp>

namespace Ck
{
	LogManager::LogManager() :
		mLevel(LogLevel::Info)
	{
		/// Nothing
	}

	void LogManager::RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel)
	{
		logChannel->SetLevel(mLevel);
		mChannels[name] = std::move(logChannel);
	}

	LogLevel LogManager::GetLevel() const
	{
		return mLevel;
	}

	void LogManager::SetLevel(LogLevel level)
	{
		mLevel = level;
	}
}
