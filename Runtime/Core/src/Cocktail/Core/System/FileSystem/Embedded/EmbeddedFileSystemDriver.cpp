#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddFile.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

namespace Ck
{
	bool EmbeddedFileSystemDriver::IsFile(const std::filesystem::path& path) const
	{
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_file(path.string()))
				return true;
		}

		return false;
	}

	bool EmbeddedFileSystemDriver::IsDirectory(const std::filesystem::path& path) const
	{
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_directory(path.string()))
				return true;
		}

		return false;
	}

	void EmbeddedFileSystemDriver::CreateFile(const std::filesystem::path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::CreateDirectory(const std::filesystem::path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	Ref<File> EmbeddedFileSystemDriver::OpenFile(const std::filesystem::path& path, const FileOpenFlags& flags)
	{
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_file(path.string()))
				return EmbeddedFile::New(fileSystem, path, flags);
		}

		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	Ref<Directory> EmbeddedFileSystemDriver::OpenDirectory(const std::filesystem::path& path)
	{
		for (const cmrc::embedded_filesystem& mFileSystem : mFileSystems)
		{
			if (mFileSystem.is_directory(path.string()))
				return EmbeddedDirectory::New(mFileSystem, path);
		}

		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	void EmbeddedFileSystemDriver::Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::Move(const std::filesystem::path& source, const std::filesystem::path& destination)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::Remove(const std::filesystem::path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::Register(cmrc::embedded_filesystem fileSystem)
	{
		mFileSystems.push_back(std::move(fileSystem));
	}
}
