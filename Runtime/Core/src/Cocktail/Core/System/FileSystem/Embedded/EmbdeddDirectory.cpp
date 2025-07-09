#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>

namespace Ck
{
	EmbeddedDirectory::EmbeddedDirectory(cmrc::embedded_filesystem fileSystem, std::filesystem::path path):
		mFileSystem(fileSystem),
		mPath(std::move(path))
	{
		/// Nothing
	}

	Array<std::filesystem::path> EmbeddedDirectory::GetContent() const
	{
		Array<std::filesystem::path> childrens;
		cmrc::directory_iterator iterable = mFileSystem.iterate_directory(mPath.string());

		std::filesystem::path basePath = "builtin://" / mPath;

		childrens.Reserve(std::distance(iterable.begin(), iterable.end()));
		for (auto it = iterable.begin(); it != iterable.end(); ++it)
			childrens.Emplace(basePath.string() + '/' + (*it).filename());

		return childrens;
	}

	const std::filesystem::path& EmbeddedDirectory::GetPath() const
	{
		return mPath;
	}

	void* EmbeddedDirectory::GetSystemHandle() const
	{
		return nullptr;
	}
}
