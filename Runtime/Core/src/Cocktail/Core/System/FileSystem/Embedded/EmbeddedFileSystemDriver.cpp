#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddFile.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	bool EmbeddedFileSystemDriver::IsFile(const Path& path) const
	{
		std::string p = CK_TEXT_TO_ANSI(path.ToString().GetData());
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_file(p))
				return true;
		}

		return false;
	}

	bool EmbeddedFileSystemDriver::IsDirectory(const Path& path) const
	{
		std::string p = CK_TEXT_TO_ANSI(path.ToString().GetData());
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_directory(p))
				return true;
		}

		return false;
	}

	void EmbeddedFileSystemDriver::CreateFile(const Path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::CreateDirectory(const Path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	std::unique_ptr<File> EmbeddedFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
	{
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_file(CK_TEXT_TO_ANSI(path.ToString().GetData())))
				return std::make_unique<EmbeddedFile>(fileSystem, path, flags);
		}

		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	std::unique_ptr<Directory> EmbeddedFileSystemDriver::OpenDirectory(const Path& path)
	{
		for (const cmrc::embedded_filesystem& mFileSystem : mFileSystems)
		{
			if (mFileSystem.is_directory(CK_TEXT_TO_ANSI(path.ToString().GetData())))
				return std::make_unique<EmbeddedDirectory>(mFileSystem, path);
		}

		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	void EmbeddedFileSystemDriver::Copy(const Path& source, const Path& destination, bool failIfExists)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::Move(const Path& source, const Path& destination)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::Remove(const Path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	Path EmbeddedFileSystemDriver::MakeCanonical(const Path& path)
	{
		return path;
	}

	Optional<Path> EmbeddedFileSystemDriver::TryMakeCanonical(const Path& path)
	{
		return Optional<Path>::Of(path);
	}

	void EmbeddedFileSystemDriver::Register(cmrc::embedded_filesystem fileSystem)
	{
		mFileSystems.Add(std::move(fileSystem));
	}
}
