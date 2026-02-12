#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>

namespace Ck
{
	EmbeddedDirectory::EmbeddedDirectory(cmrc::embedded_filesystem fileSystem, Path path):
		mFileSystem(fileSystem),
		mPath(std::move(path))
	{
	    mPath.ToFormatInPlace(Path::Format::Generic);
		mUtf8Path = Utf8String::Convert(mPath.ToString());
	}

	Array<Path> EmbeddedDirectory::GetContent() const
	{
		Array<Path> children;
		cmrc::directory_iterator iterable = mFileSystem.iterate_directory(reinterpret_cast<const AnsiChar*>(mUtf8Path.GetData()));

		children.Reserve(std::distance(iterable.begin(), iterable.end()));
		for (auto it = iterable.begin(); it != iterable.end(); ++it)
		{
			AsciiString filename = (*it).filename().c_str();

		    Path child = mPath;
		    child.Join(String::Convert(filename));

		    String test = child.ToString();

			children.Add(std::move(child));
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
