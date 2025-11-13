#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	ConsoleLogChannel::ConsoleLogChannel(ConsoleService* console):
		mConsole(console)
	{
		SetLevelColors(LogLevel::Debug, ConsoleColor::White, ConsoleColor::Transparent);
		SetLevelColors(LogLevel::Info, ConsoleColor::Blue, ConsoleColor::Transparent);
		SetLevelColors(LogLevel::Warning, ConsoleColor::Yellow, ConsoleColor::Transparent);
		SetLevelColors(LogLevel::Error, ConsoleColor::Red, ConsoleColor::Transparent);
		SetLevelColors(LogLevel::Critical, ConsoleColor::Red, ConsoleColor::Transparent, ConsoleStyle::Bold);
	}

	void ConsoleLogChannel::SetLevelColors(LogLevel level, ConsoleColor text, ConsoleColor background, ConsoleStyle style)
	{
		mColors[level] = LevelColors{ text, background, style };
	}

	void ConsoleLogChannel::WriteEntry(const LogEntry& entry)
	{
		const auto& [text, background, style] = mColors[entry.Level];

		mConsole->SetColors(text, background, style);
		mConsole->WriteLine(String::Format(CK_TEXT("%s - %s"), entry.Category->GetName(), entry.Message));
	}
}
