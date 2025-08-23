#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFile.hpp>
#include <Cocktail/Core/System/SystemError.hpp>

namespace Ck::Detail::Unix
{
	LocalFile::LocalFile(const std::filesystem::path& path, const FileOpenFlags& flags) :
		mPath(path)
	{
		int sysFlags = 0;
		if (flags & (FileOpenFlagBits::Read | FileOpenFlagBits::Write))
		{
			sysFlags |= O_RDWR;
		}
		else
		{
			if (flags & FileOpenFlagBits::Read)
				sysFlags |= O_RDONLY;

			if (flags & FileOpenFlagBits::Write)
				sysFlags |= O_WRONLY;
		}

		if (flags & FileOpenFlagBits::Write && flags & FileOpenFlagBits::Truncate)
			sysFlags |= O_TRUNC;

		mHandle = ::open64(path.c_str(), sysFlags, S_IRWXU);
		if (mHandle == -1)
			throw SystemError::GetLastError();
	}

	LocalFile::~LocalFile()
	{
		::close(mHandle);
	}

	unsigned int LocalFile::Read(void* buffer, unsigned int length)
	{
		int read = ::read(mHandle, buffer, length);
		if (read == -1)
			throw SystemError::GetLastError();

		return read;
	}

	unsigned int LocalFile::Write(const void* buffer, unsigned int length)
	{
		int written = ::write(mHandle, buffer, length);
		if (written == -1)
			throw SystemError::GetLastError();

		return written;
	}

	Uint64 LocalFile::GetCursor() const
	{
		return lseek64(mHandle, 0, SEEK_CUR);
	}

	Uint64 LocalFile::SetCursor(FileCursorMode mode, Int64 offset)
	{
		switch (mode)
		{
		case FileCursorMode::Begin:
			return lseek64(mHandle, offset, SEEK_SET);

		case FileCursorMode::Current:
			return lseek64(mHandle, offset, SEEK_CUR);

		case FileCursorMode::End:
			return lseek64(mHandle, offset, SEEK_END);
		}

		COCKTAIL_UNREACHABLE();
	}

	std::size_t LocalFile::GetSize() const
	{
        struct stat64 info;
		int error = fstat64(mHandle, &info);
		if (error == -1)
			throw SystemError::GetLastError();

		return static_cast<Uint64>(info.st_size);
	}

	const std::filesystem::path& LocalFile::GetPath() const
	{
		return mPath;
	}

	void LocalFile::Flush() const
	{
		if (fsync(mHandle) != -1)
			throw SystemError::GetLastError();
	}

	void* LocalFile::GetSystemHandle() const
	{
		return reinterpret_cast<void*>(mHandle);
	}
}
