#include <cstring>

#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddFile.hpp>

namespace Ck
{
	EmbeddedFile::EmbeddedFile(const cmrc::embedded_filesystem& fileSystem, const std::filesystem::path& path, const FileOpenFlags& flags):
		mPath(path)
	{
		if (flags & FileOpenFlagBits::Truncate | flags & FileOpenFlagBits::Write)
			throw std::system_error(std::make_error_code(std::errc::read_only_file_system));

		mHandle = fileSystem.open(path.string());
		mCursor = flags & FileOpenFlagBits::Append ? mHandle.cend() : mHandle.cbegin();
	}

	unsigned EmbeddedFile::Read(void* buffer, unsigned length)
	{
		unsigned int copySize = std::min(static_cast<unsigned int>(mHandle.cend() - mCursor), length);

		std::memcpy(buffer, mCursor, copySize);

		return copySize;
	}

	unsigned EmbeddedFile::Write(const void* buffer, unsigned length)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	Uint64 EmbeddedFile::GetCursor() const
	{
		return mCursor - mHandle.cbegin();
	}

	Uint64 EmbeddedFile::SetCursor(FileCursorMode mode, Int64 offset)
	{
		switch (mode)
		{
		case FileCursorMode::Begin:
			mCursor = mHandle.cbegin() + offset;
			break;

		case FileCursorMode::Current:
			mCursor = mCursor + offset;
			break;

		case FileCursorMode::End:
			mCursor = mHandle.cend() - offset;
			break;
		}

		return GetCursor();
	}

	std::size_t EmbeddedFile::GetSize() const
	{
		return mHandle.size();
	}

	const std::filesystem::path& EmbeddedFile::GetPath() const
	{
		return mPath;
	}

	void EmbeddedFile::Flush() const
	{
		/// Nothing
	}

	void* EmbeddedFile::GetSystemHandle() const
	{
		return (void*)&mHandle;
	}
}
