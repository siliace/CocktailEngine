#ifndef COCKTAILENGINE_MAIN_MAIN_HPP
#define COCKTAILENGINE_MAIN_MAIN_HPP

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>

#include <CocktailEngine/Main/ExitCode.hpp>

namespace Ck::Main
{
	COCKTAIL_DECLARE_LOG_CATEGORY(MainLogCategory, LogLevel::Info);

	/**
	 * \brief 
	 * \param application 
	 * \return 
	 */
	ExitCode InvokeMain(Ck::UniquePtr<Ck::Application> application);
}

#endif // COCKTAILENGINE_MAIN_MAIN_HPP
