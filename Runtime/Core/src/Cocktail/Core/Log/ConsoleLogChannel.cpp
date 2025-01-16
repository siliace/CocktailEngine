#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>

namespace Ck
{
	ConsoleLogChannel::ConsoleLogChannel(Ref<ConsoleService> console):
		mConsole(std::move(console))
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
		mConsole->GetOutput().Write(entry.Message.c_str(), entry.Message.length());
	}
}
