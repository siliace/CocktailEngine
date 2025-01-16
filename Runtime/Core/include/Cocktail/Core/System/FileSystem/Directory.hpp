#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORY_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORY_HPP

#include <filesystem>

#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class Directory : public Implements<Directory, Interface>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::vector<std::filesystem::path> GetContent() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const std::filesystem::path& GetPath() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORY_HPP
