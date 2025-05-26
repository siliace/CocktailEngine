#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP

namespace Ck
{
	/**
	 * \brief 
	 */
	class FileLock
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~FileLock() = default;

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
