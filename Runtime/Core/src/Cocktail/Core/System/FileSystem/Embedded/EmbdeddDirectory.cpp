#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>

namespace Ck
{
	EmbeddedDirectory::EmbeddedDirectory(cmrc::embedded_filesystem fileSystem, Path path):
		mFileSystem(fileSystem),
		mPath(std::move(path))
	{
		mUtf8Path = Utf8String::Convert(path.ToString());
	}

	Array<Path> EmbeddedDirectory::GetContent() const
	{
		Array<Path> children;
		cmrc::directory_iterator iterable = mFileSystem.iterate_directory(reinterpret_cast<const AnsiChar*>(mUtf8Path.GetData()));

		children.Reserve(std::distance(iterable.begin(), iterable.end()));
		for (auto it = iterable.begin(); it != iterable.end(); ++it)
		{
			AsciiStringView filename = (*it).filename().c_str();
			children.Add(Path::Merge(mPath, String::Convert(filename)));
		}

		return children;
	}

	const Path& EmbeddedDirectory::GetPath() const
	{
		return mPath;
	}

	void* EmbeddedDirectory::GetSystemHandle() const
	{
		return nullptr;
	}
}
