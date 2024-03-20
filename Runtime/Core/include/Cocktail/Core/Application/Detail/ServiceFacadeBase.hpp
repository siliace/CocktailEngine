#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP

#include <Cocktail/Core/Application/Application.hpp>

namespace Ck::Detail
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API ServiceFacadeBase
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static Application* GetApplicationFacade();

		/**
		 * \brief 
		 * \param application 
		 */
		static void SetApplicationFacade(Application* application);

	private:

		static Application* sFacadeApplication;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_SERVICEFACADEBASE_HPP
