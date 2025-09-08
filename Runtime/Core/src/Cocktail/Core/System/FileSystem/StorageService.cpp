#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>

namespace Ck
{
	StorageService::StorageService(String defaultScheme):
		mDefaultScheme(std::move(defaultScheme))
	{
		assert(!mDefaultScheme.IsEmpty());
	}

	bool StorageService::IsFile(const URI& uri) const
	{
		return ResolveDriver(uri.GetScheme())->IsFile(uri.GetPath());
	}

	bool StorageService::IsDirectory(const URI& uri) const
	{
		return ResolveDriver(uri.GetScheme())->IsDirectory(uri.GetPath());
	}

	void StorageService::CreateFile(const URI& uri) const
	{
		return ResolveDriver(uri.GetScheme())->CreateFile(uri.GetPath());
	}

	void StorageService::CreateDirectory(const URI& uri) const
	{
		return ResolveDriver(uri.GetScheme())->CreateDirectory(uri.GetPath());
	}

	std::unique_ptr<File> StorageService::OpenFile(const URI& uri, FileOpenFlags flags) const
	{
		return ResolveDriver(uri.GetScheme())->OpenFile(uri.GetPath(), flags);
	}

	std::unique_ptr<Directory> StorageService::OpenDirectory(const URI& uri) const
	{
		return ResolveDriver(uri.GetScheme())->OpenDirectory(uri.GetPath());
	}

	void StorageService::Copy(const URI& source, const URI& destination, bool failIfExists) const
	{
		FileSystemDriver* sourceDriver = ResolveDriver(source.GetScheme());
		FileSystemDriver* destinationDriver = ResolveDriver(destination.GetScheme());

		if (sourceDriver == destinationDriver)
		{
			sourceDriver->Copy(source.GetPath(), destination.GetPath(), failIfExists);
		}
		else
		{
			std::unique_ptr<File> sourceFile = sourceDriver->OpenFile(source.GetPath(), FileOpenFlagBits::Read);

			FileOpenFlags destinationOpenFlags = FileOpenFlagBits::Write | FileOpenFlagBits::Truncate;
			if (failIfExists)
				destinationOpenFlags |= FileOpenFlagBits::Existing;

			std::unique_ptr<File> destinationFile = destinationDriver->OpenFile(destination.GetPath(), destinationOpenFlags);

			const std::size_t bufferSize = 2048;
			unsigned char buffer[bufferSize] = { 0 };

			FileInputStream inputStream(*sourceFile);
			FileOutputStream outputStream(*destinationFile);

			while (!inputStream.IsEof())
			{
				std::size_t read = inputStream.Read(buffer, bufferSize);
				outputStream.Write(buffer, read);
			}
		}
	}

	void StorageService::Move(const URI& source, const URI& destination) const
	{
		FileSystemDriver* sourceDriver = ResolveDriver(source.GetScheme());
		FileSystemDriver* destinationDriver = ResolveDriver(destination.GetScheme());

		if (sourceDriver == destinationDriver)
		{
			sourceDriver->Move(source.GetPath(), destination.GetPath());
		}
		else
		{
			Copy(source, destination, false);
			sourceDriver->Remove(source.GetPath());
		}
	}

	void StorageService::Remove(const URI& uri) const
	{
		return ResolveDriver(uri.GetScheme())->Remove(uri.GetPath());
	}

	void StorageService::Mount(String scheme, FileSystemDriver* fileSystemDriver)
	{
		mDrivers.insert_or_assign(std::move(scheme), fileSystemDriver);
	}

	void StorageService::UnMount(const String& scheme)
	{
		mDrivers.erase(scheme);
	}

	const String& StorageService::GetDefaultScheme() const
	{
		return mDefaultScheme;
	}

	FileSystemDriver* StorageService::ResolveDriver(const String& scheme) const
	{
		if (scheme.IsEmpty())
			return ResolveDriver(mDefaultScheme);

		if (auto it = mDrivers.find(scheme); it != mDrivers.end())
			return it->second;

		return nullptr;
	}
}
