#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck::Detail::Unix
{
	LocalDirectory::LocalDirectory(const Path& path) :
		mPath(path)
	{
		mHandle = opendir(CK_TEXT_TO_ANSI(path.ToString().GetData()));
	}

	LocalDirectory::~LocalDirectory()
	{
		closedir(mHandle);
	}

	Array<Path> LocalDirectory::GetContent() const
	{
		dirent64* result;
		Array<Path> content;

		while ((result = readdir64(mHandle)))
		{
			String filename = CK_ANSI_TO_TEXT(&result->d_name[0]);

			Path childPath = mPath;
			content.Add(childPath.Join(filename));
		}

		return content;
	}

	const Path &LocalDirectory::GetPath() const
	{
		return mPath;
	}

	void *LocalDirectory::GetSystemHandle() const
	{
		return (void*)mHandle;
	}
}
