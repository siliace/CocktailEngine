#include <Cocktail/Core/IO/FileOutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/FileSystem.hpp>

namespace Ck
{
	FileOutputStream::FileOutputStream(const std::string& path, bool truncate)
	{
		FileOpenFlags openFlags = FileOpenFlagBits::Write;
		if (truncate)
			openFlags |= FileOpenFlagBits::Truncate;

		mFile = FileSystem::OpenFile(path, openFlags);
	}

	std::size_t FileOutputStream::Write(const void* data, std::size_t length)
	{
		return mFile->Write(data, static_cast<unsigned int>(length));
	}

	void FileOutputStream::Flush()
	{
		mFile->Flush();
	}
}
