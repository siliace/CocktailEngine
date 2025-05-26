#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>

namespace Ck
{
	FileInputStream::FileInputStream(const std::shared_ptr<File>& file) :
		mFile(file)
	{
		/// Nothing
	}

	bool FileInputStream::HasCursor() const
	{
		return true;
	}

	std::size_t FileInputStream::Read(void* buffer, std::size_t length)
	{
		return mFile->Read(buffer, length);
	}

	Uint64 FileInputStream::Seek(Uint64 position)
	{
		return mFile->SetCursor(FileCursorMode::Current, position);
	}

	Uint64 FileInputStream::Tell() const
	{
		return mFile->GetCursor();
	}

	void FileInputStream::Rewind()
	{
		mFile->SetCursor(FileCursorMode::Begin, 0);
	}

	std::size_t FileInputStream::GetSize() const
	{
		return mFile->GetSize();
	}

	bool FileInputStream::IsEof() const
	{
		return mFile->GetCursor() == mFile->GetSize();
	}
}
