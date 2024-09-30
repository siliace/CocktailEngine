#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Log/NullLogChannel.hpp>

namespace Ck
{
	LogManager::LogManager() :
		mLevel(LogLevel::Info)
	{
		/// Nothing
	}

	void LogManager::RegisterChannel(const std::string& name, Ref<LogChannel> logChannel)
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
