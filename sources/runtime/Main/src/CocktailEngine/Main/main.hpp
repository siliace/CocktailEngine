#ifndef COCKTAILENGINE_MAIN_MAIN_HPP
#define COCKTAILENGINE_MAIN_MAIN_HPP

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>

#include <CocktailEngine/Main/ExitCode.hpp>

namespace Ck::Main
{
	COCKTAIL_DECLARE_LOG_CATEGORY(MainLogCategory, LogLevel::Info);

	/**
	 * \brief Boot the application, run the program, and tear everything down
	 *
	 * Called by the entry point of the platform, which has nothing to do but build the
	 * Application matching it and hand it over.
	 *
	 * \param application The application to run, owned from here on
	 *
	 * \return The exit code of the process. Not an ExitCode: that enumeration is the
	 *         vocabulary of ApplicationMain, while a code requested through
	 *         Application::Exit is any value the program chose.
	 */
	int InvokeMain(Ck::UniquePtr<Ck::Application> application);
}

#endif // COCKTAILENGINE_MAIN_MAIN_HPP
