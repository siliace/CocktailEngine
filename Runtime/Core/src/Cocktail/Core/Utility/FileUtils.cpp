#include <Cocktail/Core/IO/Output/Writer/FileWriter.hpp>
#include <Cocktail/Core/IO/Output/Writer/LineWriter.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

namespace Ck
{
	void FileUtils::MakeDirectories(const Path& path, FileSystemDriver* driver)
	{
		if (driver->IsDirectory(path))
            return;

		driver->CreateDirectory(path.GetParent());
		driver->CreateDirectory(path);
	}

	ByteArray FileUtils::ReadFile(const Path& path, FileSystemDriver* driver)
	{
		ByteArray content;
		if (!driver->IsFile(path))
			return content;

		UniquePtr<File> file = driver->OpenFile(path, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);

		const std::size_t size = file->GetSize();
		if (!size)
			return content;

		content.Resize(size);
		file->Read(content.GetData(), content.GetSize());

		return content;
	}

	void FileUtils::WriteFile(const Path& path, ByteArrayView content, bool append, FileSystemDriver* driver)
	{
		if (!driver->IsFile(path))
		{
			MakeDirectories(path.GetParent());
			driver->CreateFile(path);
		}

	    FileOutputStream<> fileOutputStream(path, !append);
	    fileOutputStream.Write(content.GetData(), content.GetSize());
	}
}
