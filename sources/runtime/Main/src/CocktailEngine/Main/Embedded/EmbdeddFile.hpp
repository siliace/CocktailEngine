#ifndef COCKTAILENGINE_MAIN_EMBEDDED_EMBEDDEDFILE_HPP
#define COCKTAILENGINE_MAIN_EMBEDDED_EMBEDDEDFILE_HPP

#include <CocktailEngine/Core/System/FileSystem/File.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileOpenFlags.hpp>
#include <CocktailEngine/Main/Embedded/EmbeddedFileSystemDriver.hpp>

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
		EmbeddedFile(const cmrc::embedded_filesystem& fileSystem, const Path& path, const FileOpenFlags& flags);

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
		const Path& GetPath() const override;

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

		Path mPath;
		cmrc::file mHandle;
		cmrc::file::const_iterator mCursor;
	};
}

#endif // COCKTAILENGINE_MAIN_EMBEDDED_EMBEDDEDFILE_HPP
