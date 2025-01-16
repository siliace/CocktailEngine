#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>

namespace Ck
{
	StorageService::StorageService(std::string defaultProtocol):
		mDefaultProtocol(std::move(defaultProtocol))
	{
		assert(!mDefaultProtocol.empty());
	}

	bool StorageService::IsFile(const std::filesystem::path& path) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->IsFile(p);
	}

	bool StorageService::IsDirectory(const std::filesystem::path& path) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->IsDirectory(p);
	}

	void StorageService::CreateFile(const std::filesystem::path& path) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->CreateFile(p);
	}

	void StorageService::CreateDirectory(const std::filesystem::path& path) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->CreateDirectory(p);
	}

	Ref<File> StorageService::OpenFile(const std::filesystem::path& path, FileOpenFlags flags) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->OpenFile(p, flags);
	}

	Ref<Directory> StorageService::OpenDirectory(const std::filesystem::path& path) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->OpenDirectory(p);
	}

	void StorageService::Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists) const
	{
		const auto [sourceProtocol, sourcePath] = SplitPathProtocol(source);
		const auto [destinationProtocol, destinationPath] = SplitPathProtocol(destination);

		FileSystemDriver* sourceDriver = ResolveProtocol(sourceProtocol);
		FileSystemDriver* destinationDriver = ResolveProtocol(destinationProtocol);

		if (sourceDriver == destinationDriver)
		{
			sourceDriver->Copy(sourcePath, destinationPath, failIfExists);
		}
		else
		{
			Ref<File> sourceFile = sourceDriver->OpenFile(sourcePath, FileOpenFlagBits::Read);

			FileOpenFlags destinationOpenFlags = FileOpenFlagBits::Write | FileOpenFlagBits::Truncate;
			if (failIfExists)
				destinationOpenFlags |= FileOpenFlagBits::Existing;

			Ref<File> destinationFile = destinationDriver->OpenFile(destinationPath, destinationOpenFlags);

			const std::size_t bufferSize = 2048;
			unsigned char buffer[bufferSize] = { 0 };

			FileInputStream inputStream(sourceFile);
			FileOutputStream outputStream(destinationFile);

			while (!inputStream.IsEof())
			{
				std::size_t read = inputStream.Read(buffer, bufferSize);
				outputStream.Write(buffer, read);
			}
		}
	}

	void StorageService::Move(const std::filesystem::path& source, const std::filesystem::path& destination) const
	{
		const auto [sourceProtocol, sourcePath] = SplitPathProtocol(source);
		const auto [destinationProtocol, destinationPath] = SplitPathProtocol(destination);

		FileSystemDriver* sourceDriver = ResolveProtocol(sourceProtocol);
		FileSystemDriver* destinationDriver = ResolveProtocol(destinationProtocol);

		if (sourceDriver == destinationDriver)
		{
			sourceDriver->Move(sourcePath, destinationPath);
		}
		else
		{
			Copy(source, destination, false);
			sourceDriver->Remove(sourcePath);
		}
	}

	void StorageService::Remove(const std::filesystem::path& path) const
	{
		const auto [protocol, p] = SplitPathProtocol(path);
		return ResolveProtocol(protocol)->Remove(p);
	}

	void StorageService::Mount(std::string protocol, FileSystemDriver* fileSystemDriver)
	{
		mDrivers.insert_or_assign(std::move(protocol), fileSystemDriver);
	}

	void StorageService::UnMount(const std::string& protocol)
	{
		mDrivers.erase(protocol);
	}

	const std::string& StorageService::GetDefaultProtocol() const
	{
		return mDefaultProtocol;
	}

	std::pair<std::string, std::filesystem::path> StorageService::SplitPathProtocol(const std::filesystem::path& path)
	{
		std::string pathAsString = path.string();
		std::size_t protocolSeparatorIndex = pathAsString.find(ProtocolSeparator);
		if (protocolSeparatorIndex == std::string::npos)
			return std::make_pair("", path);

		std::string protocol = pathAsString.substr(0, protocolSeparatorIndex);
		std::filesystem::path splitPath = pathAsString.substr(protocolSeparatorIndex + ProtocolSeparator.length());

		return std::make_pair(std::move(protocol), std::move(splitPath));
	}

	FileSystemDriver* StorageService::ResolveProtocol(const std::string& protocol) const
	{
		if (protocol.empty())
			return ResolveProtocol(mDefaultProtocol);

		return mDrivers.at(protocol);
	}
}
