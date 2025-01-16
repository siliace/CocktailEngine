#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/ConsoleLogChannel.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Log/LogServiceProvider.hpp>

namespace Ck
{
	LogServiceProvider::LogServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		/// Nothing
	}

	void LogServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<LogManager>();
	}

	void LogServiceProvider::DoBoot(Application* application)
	{
		application->Invoke([](LogManager* logManager, ConsoleService* console) {
			logManager->RegisterChannel("console", std::make_unique<ConsoleLogChannel>(console));
		});
	}
}
