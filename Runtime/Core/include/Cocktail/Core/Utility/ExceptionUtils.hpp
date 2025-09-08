#ifndef COCKTAIL_CORE_UTILITY_EXCEPTIONUTILS_HPP
#define COCKTAIL_CORE_UTILITY_EXCEPTIONUTILS_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief Utility class creating and throwing exceptions
	 */
	class COCKTAIL_CORE_API ExceptionUtils
	{
	public:

		/**
		 * \brief Throws an NotImplemented error
		 */
		[[noreturn]] static void ThrowNotImplemented();

		/**
		 * \brief Throws an EmptyOptionalException error
		 */
		[[noreturn]] static void ThrowEmptyOptional();

		/**
		 * \brief Throws an EmptyOptionalException error
		 * \param message The message to use to create the exception thrown
		 */
		[[noreturn]] static void ThrowEmptyOptional(const TextChar* message);

		/**
		 * \brief Throws an ContainerEmpty error
		 */
		[[noreturn]] static void ThrowEmptyContainer();

		/**
		 * \brief Throws an ContainerEmpty error
		 * \param message The message to use to create the exception thrown
		 */
		[[noreturn]] static void ThrowEmptyContainer(const TextChar* message);

		/**
		 * \brief Throws an ContainerOutOfRange error
		 * This exception with be formatted using \p index and \p max
		 * \param index The index used 
		 * \param max The size of the container
		 */
		[[noreturn]] static void ThrowOutOfRange(Uint32 index, Uint32 max);

		/**
		 * \brief Throws an ContainerOutOfRange error
		 * This exception with be formatted using \p index and \p max
		 * \param index The index used
		 * \param max The size of the container
		 */
		[[noreturn]] static void ThrowOutOfRange(Uint64 index, Uint64 max);
	};
}

#endif // COCKTAIL_CORE_UTILITY_EXCEPTIONUTILS_HPP
