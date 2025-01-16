#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILE_HPP

#include <filesystem>

#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/System/FileSystem/FileCursorMode.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class File : public Implements<File, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param buffer 
		 * \param length 
		 * \return 
		 */
		virtual unsigned int Read(void* buffer, unsigned int length) = 0;

		/**
		 * \brief
		 * \param buffer
		 * \param length
		 * \return
		 */
		virtual unsigned int Write(const void* buffer, unsigned int length) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Uint64 GetCursor() const = 0;

		/**
		 * \brief 
		 * \param mode 
		 * \param offset 
		 * \return 
		 */
		virtual Uint64 SetCursor(FileCursorMode mode, Int64 offset) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::size_t GetSize() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const std::filesystem::path& GetPath() const = 0;

		/**
		 * \brief 
		 */
		virtual void Flush() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILE_HPP
