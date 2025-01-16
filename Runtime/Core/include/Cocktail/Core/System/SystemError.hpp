#ifndef COCKTAIL_CORE_SYSTEM_SYSTEMERROR_HPP
#define COCKTAIL_CORE_SYSTEM_SYSTEMERROR_HPP

#include <system_error>

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API SystemError
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

#endif // COCKTAIL_CORE_SYSTEM_SYSTEMERROR_HPP
