#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class FileLock : public Implements<File, Interface>
	{
	public:

		/**
		 * \brief 
		 */
		virtual void Lock() = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool TryLock() = 0;

		/**
		 * \brief 
		 */
		virtual void Unlock() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP
