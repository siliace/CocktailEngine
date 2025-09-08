#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	EmbeddedDirectory::EmbeddedDirectory(cmrc::embedded_filesystem fileSystem, Path path):
		mFileSystem(fileSystem),
		mPath(std::move(path))
	{
		/// Nothing
	}

	Array<Path> EmbeddedDirectory::GetContent() const
	{
		Array<Path> children;
		cmrc::directory_iterator iterable = mFileSystem.iterate_directory(CK_TEXT_TO_ANSI(mPath.ToString().GetData()));

		children.Reserve(std::distance(iterable.begin(), iterable.end()));
		for (auto it = iterable.begin(); it != iterable.end(); ++it)
			children.Add(mPath.Join(CK_ANSI_TO_TEXT((*it).filename().c_str())));

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
