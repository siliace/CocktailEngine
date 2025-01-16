#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILEOPENFLAGS_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILEOPENFLAGS_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	enum class FileOpenFlagBits
	{
		/**
		 * \brief Enable read operations on the File to open
		 */
		Read = Bit(0),

		/**
		 * \brief Enable write operations on the File to open
		 */
		Write = Bit(1),

		/**
		 * \brief Put the file cursor at the end of the File when opened
		 */
		Append = Bit(2),

		/**
		 * \brief Empty the File when it's opened
		 */
		Truncate = Bit(3),

		/**
		 * \brief Fail to open the file it it does not exists even combined with Write
		 */
		Existing = Bit(4),

		/**
		 * \brief Disable system buffering of I/O operations
		 */
		Unbuffered = Bit(5),

		/**
		 * \brief Forbid other processes to write the file while it's still opened
		 */
		Locked = Bit(5),
	};

	using FileOpenFlags = Flags<FileOpenFlagBits>;
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILEOPENFLAGS_HPP
