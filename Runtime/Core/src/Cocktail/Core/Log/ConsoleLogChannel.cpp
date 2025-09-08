#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>

namespace Ck
{
	ConsoleLogChannel::ConsoleLogChannel(ConsoleService* console):
		mConsole(console)
	{
		SetLevelColors(LogLevel::Debug, ConsoleColor::Black, ConsoleColor::White);
		SetLevelColors(LogLevel::Info, ConsoleColor::Black, ConsoleColor::Blue);
		SetLevelColors(LogLevel::Warning, ConsoleColor::Black, ConsoleColor::Yellow);
		SetLevelColors(LogLevel::Error, ConsoleColor::Black, ConsoleColor::Red);
		SetLevelColors(LogLevel::Critical, ConsoleColor::Red, ConsoleColor::White);
	}

	void ConsoleLogChannel::SetLevelColors(LogLevel level, ConsoleColor background, ConsoleColor foreground)
	{
		mColors[level] = std::make_pair(background, foreground);
	}

	void ConsoleLogChannel::WriteEntry(const LogEntry& entry)
	{
		const auto& [background, foreground] = mColors[entry.Level];

		mConsole->SetColors(background, foreground);

		std::string fullMessage = ""; /// TODO: fmt::format("{} - {}\n", "TODO", entry.entry.LogCategory->GetName()); 
		mConsole->GetOutput().Write(fullMessage.c_str(), fullMessage.length());
	}
}
