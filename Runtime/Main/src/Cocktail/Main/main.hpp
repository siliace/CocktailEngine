#ifndef COCKTAIL_MAIN_MAIN_HPP
#define COCKTAIL_MAIN_MAIN_HPP

#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Main/ExitCode.hpp>

namespace Ck::Main
{
	/**
	 * \brief 
	 * \param application 
	 * \return 
	 */
	ExitCode InvokeMain(Ck::Application* application);
}

#endif // COCKTAIL_MAIN_MAIN_HPP
