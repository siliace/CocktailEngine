#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>

namespace Ck
{
	FileOutputStream::FileOutputStream(File& file) :
		mFile(&file)
	{
		/// Nothing
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
