#ifndef COCKTAIL_MAIN_MAIN_HPP
#define COCKTAIL_MAIN_MAIN_HPP

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>

#include <Cocktail/Main/ExitCode.hpp>

namespace Ck::Main
{
	COCKTAIL_DECLARE_LOG_CATEGORY(MainLogCategory, LogLevel::Info);

	/**
	 * \brief 
	 * \param application 
	 * \return 
	 */
	ExitCode InvokeMain(Ck::Application* application);
}

#endif // COCKTAIL_MAIN_MAIN_HPP
