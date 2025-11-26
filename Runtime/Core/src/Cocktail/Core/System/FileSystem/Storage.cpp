#include <Cocktail/Core/System/FileSystem/Storage.hpp>

namespace Ck
{
	bool Storage::IsFile(const URI& uri)
	{
		return ResolveFacadeInstance()->IsFile(uri);
	}

	bool Storage::IsDirectory(const URI& uri)
	{
		return ResolveFacadeInstance()->IsDirectory(uri);
	}

	void Storage::CreateFile(const URI& uri)
	{
		return ResolveFacadeInstance()->CreateFile(uri);
	}

	void Storage::CreateDirectory(const URI& uri)
	{
		return ResolveFacadeInstance()->CreateDirectory(uri);
	}

	UniquePtr<File> Storage::OpenFile(const URI& uri, FileOpenFlags flags)
	{
		return ResolveFacadeInstance()->OpenFile(uri, flags);
	}

    UniquePtr<Directory> Storage::OpenDirectory(const URI& uri)
	{
		return ResolveFacadeInstance()->OpenDirectory(uri);
	}

	void Storage::Copy(const URI& source, const URI& destination, bool failIfExists)
	{
		return ResolveFacadeInstance()->Copy(source, destination, failIfExists);
	}

	void Storage::Move(const URI& source, const URI& destination)
	{
		return ResolveFacadeInstance()->Move(source, destination);
	}

	void Storage::Remove(const URI& uri)
	{
		return ResolveFacadeInstance()->Remove(uri);
	}

	void Storage::Mount(String protocol, UniquePtr<FileSystemDriver> fileSystemDriver)
	{
		ResolveFacadeInstance()->Mount(std::move(protocol), std::move(fileSystemDriver));
	}

	void Storage::MountExternal(String protocol, FileSystemDriver* fileSystemDriver)
	{
		ResolveFacadeInstance()->MountExternal(std::move(protocol), fileSystemDriver);
	}

	void Storage::UnMount(const String& protocol)
	{
		ResolveFacadeInstance()->UnMount(protocol);
	}
    FileSystemDriver* Storage::ResolveDriver(const String& scheme)
    {
	    return ResolveFacadeInstance()->ResolveDriver(scheme);
    }
}
