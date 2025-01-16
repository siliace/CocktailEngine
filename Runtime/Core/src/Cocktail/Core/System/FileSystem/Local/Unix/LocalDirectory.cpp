#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp>

namespace Ck::Detail::Unix
{
	LocalDirectory::LocalDirectory(const std::filesystem::path& path) : 
		mPath(path)
	{
		mHandle = opendir(path.c_str());
	}

	LocalDirectory::~LocalDirectory()
	{
		closedir(mHandle);
	}

	std::vector<std::filesystem::path> LocalDirectory::GetContent() const
	{
		dirent64* result;
		std::vector<std::filesystem::path> content;

		while ((result = readdir64(mHandle)))
			content.emplace_back(mPath / result->d_name);

		return content;
	}

	const std::filesystem::path &LocalDirectory::GetPath() const
	{
		return mPath;
	}

	void *LocalDirectory::GetSystemHandle() const
	{
		return (void*)mHandle;
	}
}
