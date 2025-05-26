#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILE_HPP

#include <Cocktail/Core/System/FileSystem/File.hpp>
#include <Cocktail/Core/System/FileSystem/FileOpenFlags.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class EmbeddedFile final : public File
	{
	public:

		/**
		 * \brief 
		 * \param fileSystem 
		 * \param path 
		 * \param flags 
		 */
		EmbeddedFile(const cmrc::embedded_filesystem& fileSystem, const std::filesystem::path& path, const FileOpenFlags& flags);

		/**
		 * \brief 
		 * \param buffer 
		 * \param length 
		 * \return 
		 */
		unsigned int Read(void* buffer, unsigned int length) override;

		/**
		 * \brief 
		 * \param buffer 
		 * \param length 
		 * \return 
		 */
		unsigned int Write(const void* buffer, unsigned int length) override;

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 GetCursor() const override;

		/**
		 * \brief 
		 * \param mode 
		 * \param offset 
		 * \return 
		 */
		Uint64 SetCursor(FileCursorMode mode, Int64 offset) override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		const std::filesystem::path& GetPath() const override;

		/**
		 * \brief 
		 */
		void Flush() const override;

		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:

		std::filesystem::path mPath;
		cmrc::file mHandle;
		cmrc::file::const_iterator mCursor;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILE_HPP
