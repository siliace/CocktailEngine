#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Log/ConsoleLogChannel.hpp>
#include <CocktailEngine/Core/Log/LogManager.hpp>
#include <CocktailEngine/Core/Log/LogServiceProvider.hpp>

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
			logManager->RegisterChannel(CK_TEXT("console"), MakeUnique<ConsoleLogChannel>(console));
		});
	}
}
