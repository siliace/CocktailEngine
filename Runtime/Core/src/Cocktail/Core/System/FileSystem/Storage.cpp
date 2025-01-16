#include <Cocktail/Core/System/FileSystem/Storage.hpp>

namespace Ck
{
	bool Storage::IsFile(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->IsFile(path);
	}

	bool Storage::IsDirectory(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->IsDirectory(path);
	}

	void Storage::CreateFile(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->CreateFile(path);
	}

	void Storage::CreateDirectory(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->CreateDirectory(path);
	}

	Ref<File> Storage::OpenFile(const std::filesystem::path& path, FileOpenFlags flags)
	{
		return ResolveFacadeInstance()->OpenFile(path, flags);
	}

	Ref<Directory> Storage::OpenDirectory(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->OpenDirectory(path);
	}

	void Storage::Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists)
	{
		return ResolveFacadeInstance()->Copy(source, destination, failIfExists);
	}

	void Storage::Move(const std::filesystem::path& source, const std::filesystem::path& destination)
	{
		return ResolveFacadeInstance()->Move(source, destination);
	}

	void Storage::Remove(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->Remove(path);
	}

	void Storage::Mount(std::string protocol, FileSystemDriver* fileSystemDriver)
	{
		ResolveFacadeInstance()->Mount(std::move(protocol), std::move(fileSystemDriver));
	}

	void Storage::UnMount(const std::string& protocol)
	{
		ResolveFacadeInstance()->UnMount(protocol);
	}
}
