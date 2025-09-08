#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILE_HPP

#include <Cocktail/Core/System/FileSystem/File.hpp>
#include <Cocktail/Core/System/FileSystem/FileOpenFlags.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalFile : public File
	{
	public:

		/**
		 * \brief 
		 * \param path 
		 * \param flags 
		 */
		LocalFile(const Path& path, const FileOpenFlags& flags);

		/**
		 * \brief 
		 */
		~LocalFile() override;

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
		int mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILE_HPP
