#ifndef COCKTAILENGINE_CORE_SYSTEM_SYSTEMERROR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_SYSTEMERROR_HPP

#include <system_error>

#include <CocktailEngine/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API SystemError
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static std::system_error GetLastError();

		/**
		 * \brief 
		 * \return 
		 */
		static const std::error_category& GetSystemErrorCategory();
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_SYSTEMERROR_HPP
