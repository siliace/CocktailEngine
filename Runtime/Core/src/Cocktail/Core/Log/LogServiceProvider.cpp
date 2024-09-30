#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Log/LogServiceProvider.hpp>
#include <Cocktail/Core/Log/NullLogChannel.hpp>

namespace Ck
{
	LogServiceProvider::LogServiceProvider(const Ref<Application>& application) :
		Super(application)
	{
		/// Nothing
	}

	void LogServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<LogManager>();
		application->Factory<NullLogChannel>();
		application->Factory<ConsoleLogChannel>([](Ref<ConsoleService> console) {
			return ConsoleLogChannel::New(std::move(console));
		});
	}

	void LogServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Invoke([](const Ref<LogManager>& logManager, Ref<ConsoleLogChannel> consoleLogChannel) {
			logManager->RegisterChannel("console", std::move(consoleLogChannel));
		});
	}
}
